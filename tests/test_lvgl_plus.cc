#include "bloc/bloc.hh"
#include "lvgl.h"
#include "test_fixture.hh"
#include "wait_for.hh"
#include "widgets/bloc_widget_builder.hh"
#include "widgets/button.hh"
#include "widgets/layouts/column.hh"
#include "widgets/layouts/container.hh"
#include "widgets/stream_widget_builder.hh"
#include "widgets/text.hh"
#include "widgets/text_field.hh"
#include <gtest/gtest.h>
#include <thread>

using namespace softeq::lvglplus;
using namespace softeq::streams;

static void closeWindow()
{
    UniqueLock locker(softeq::lvglplus::LvglPlus::lvglMutex());
    LvglPlus::stopApp();
}

static void closeWindowAfterRender(std::shared_ptr<Widget> widget)
{
    ASSERT_TRUE(waitForRender(widget));
    closeWindow();
}

static void runSimpleApp()
{
    for (int i = 0; i < 5; i++)
    {
        auto root = std::make_shared<Text>(std::to_string(i), TextParamsBuilder().alignment(Alignment::center));
        std::thread threadCloseWindow(closeWindowAfterRender, root);
        LvglPlus::setScreen(root);
        LvglPlus::runApp();
        threadCloseWindow.join();
    }
}

TEST(LvglPlusTest, LvglPlusRun)
{
    TestParams::getInstance();
    ASSERT_NO_THROW(runSimpleApp());
}

TEST(LvglPlusTest, LvglStreamWidgetBuilder)
{
    TestParams::getInstance();
    bool newWidgetBuilt = false;
    auto streamEventLoop = std::make_shared<StreamEventLoop>();
    auto navigationController = std::make_shared<StreamController<int>>(streamEventLoop);
    auto root = std::make_shared<Container>();

    std::weak_ptr<Widget> oldBuilderChild;
    std::weak_ptr<Widget> builderChild;

    root->addChild(std::make_shared<StreamWidgetBuilder<int>>(0, navigationController->stream(),
                                                              [&](int value) -> std::shared_ptr<Widget> {
                                                                  std::shared_ptr<Widget> wid;
                                                                  if (value == 0)
                                                                  {
                                                                      wid = std::make_shared<Text>();
                                                                  }
                                                                  if (value == 1)
                                                                  {
                                                                      wid = std::make_shared<Container>();
                                                                  }
                                                                  if (value == 2)
                                                                  {
                                                                      wid = std::make_shared<Column>();
                                                                  }
                                                                  oldBuilderChild = builderChild;
                                                                  builderChild = wid;
                                                                  newWidgetBuilt = true;
                                                                  return wid;
                                                              }));

    // check default screen created
    ASSERT_TRUE(std::dynamic_pointer_cast<Text>(builderChild.lock()));

    std::thread testBuilder([&]() {
        ASSERT_TRUE(waitForRender(builderChild.lock()));
        for (int i = 0; i < 7; i++)
        {
            int value = i % 3;
            newWidgetBuilt = false;
            navigationController->add(value);
            // wait until a new widget builds
            ASSERT_TRUE(waitForCondition([&]() -> bool { return newWidgetBuilt; }));
            if (value == 0)
            {
                // check screen 1 created
                ASSERT_TRUE(std::dynamic_pointer_cast<Text>(builderChild.lock()));
            }
            if (value == 1)
            {
                // check screen 2 created
                ASSERT_TRUE(std::dynamic_pointer_cast<Container>(builderChild.lock()));
            }
            if (value == 2)
            {
                // check screen 3 created
                ASSERT_TRUE(std::dynamic_pointer_cast<Column>(builderChild.lock()));
            }
            // check previous screen deleted
            ASSERT_TRUE(waitForCondition([&]() -> bool { return oldBuilderChild.expired(); }));
        }
        closeWindow();
    });

    LvglPlus::setScreen(root);
    LvglPlus::runApp();

    testBuilder.join();
}

/* Test BLoC */
class ScreenState
{
public:
    virtual ~ScreenState() = default;
};
class Screen1State : public ScreenState
{
public:
    std::string title = "Screen 1";
};
class Screen2State : public ScreenState
{
public:
    std::string title = "Screen 2";
};
class Screen3State : public ScreenState
{
public:
    std::string title = "Screen 3";
};

class ScreenEvent
{
public:
    virtual ~ScreenEvent() = default;
};
class OpenScreen1Event : public ScreenEvent
{
};
class OpenScreen2Event : public ScreenEvent
{
};
class OpenScreen3Event : public ScreenEvent
{
};

class ScreenBloc : public Bloc<ScreenEvent, ScreenState>
{
public:
    ScreenBloc()
        : Bloc<ScreenEvent, ScreenState>(Screen1State())
    {
        on<OpenScreen1Event>([this](const OpenScreen1Event &event) { this->emit(Screen1State()); });
        on<OpenScreen2Event>([this](const OpenScreen2Event &event) { this->emit(Screen2State()); });
        on<OpenScreen3Event>([this](const OpenScreen3Event &event) { this->emit(Screen3State()); });
    }

private:
};

TEST(LvglPlusTest, LvglBlocWidgetBuilder)
{
    TestParams::getInstance();
    auto screenBloc = std::make_shared<ScreenBloc>();
    auto root = std::make_shared<Container>();
    bool newWidgetBuilt = false;

    std::weak_ptr<Widget> oldBuilderChild;
    std::weak_ptr<Widget> builderChild;

    auto window = std::make_shared<BlocWidgetBuilder<ScreenBloc, ScreenState>>(
        screenBloc, [&](const std::shared_ptr<ScreenState> &state) -> std::shared_ptr<Widget> {
            std::shared_ptr<Widget> wid;
            if (auto stateSettingsPage = screenBloc->getIf<Screen1State>(state))
            {
                wid = std::make_shared<Text>();
            }
            if (auto stateSettingsPage = screenBloc->getIf<Screen2State>(state))
            {
                wid = std::make_shared<Container>();
            }
            if (auto stateSettingsPage = screenBloc->getIf<Screen3State>(state))
            {
                wid = std::make_shared<Column>();
            }
            oldBuilderChild = builderChild;
            builderChild = wid;
            newWidgetBuilt = true;
            return wid;
        });

    root->addChild(window);

    // check default screen created
    ASSERT_TRUE(std::dynamic_pointer_cast<Text>(builderChild.lock()));

    std::thread testBuilder([&]() {
        ASSERT_TRUE(waitForRender(builderChild.lock()));
        for (int i = 0; i < 7; i++)
        {
            int value = i % 3;
            newWidgetBuilt = false;
            if (value == 0)
            {
                screenBloc->add(OpenScreen1Event());
                // wait until a new widget builds
                ASSERT_TRUE(waitForCondition([&]() -> bool { return newWidgetBuilt; }));
                // check screen 1 created
                ASSERT_TRUE(std::dynamic_pointer_cast<Text>(builderChild.lock()));
            }
            if (value == 1)
            {
                screenBloc->add(OpenScreen2Event());
                // wait until a new widget builds
                ASSERT_TRUE(waitForCondition([&]() -> bool { return newWidgetBuilt; }));
                // check screen 2 created
                ASSERT_TRUE(std::dynamic_pointer_cast<Container>(builderChild.lock()));
            }
            if (value == 2)
            {
                screenBloc->add(OpenScreen3Event());
                // wait until a new widget builds
                ASSERT_TRUE(waitForCondition([&]() -> bool { return newWidgetBuilt; }));
                // check screen 3 created
                ASSERT_TRUE(std::dynamic_pointer_cast<Column>(builderChild.lock()));
            }
            // check previous screen deleted
            ASSERT_TRUE(waitForCondition([&]() -> bool { return oldBuilderChild.expired(); }));
        }
        closeWindow();
    });

    LvglPlus::setScreen(root);
    LvglPlus::runApp();

    testBuilder.join();
}

TEST(LvglPlusTest, ButtonTextFieldTextTest) // Checks button callback, text field callback, text controller
{
    TestParams::getInstance();

    std::string oldText = "old text";
    std::string newText = "new text";
    std::string textFieldText = "no changes";

    auto column = std::make_shared<Column>();
    auto myTextField = std::make_shared<TextField>(textFieldText);
    auto myText = std::make_shared<Text>(oldText);

    // check text
    ASSERT_EQ(myText->getText(), oldText);
    ASSERT_EQ(myTextField->getText(), textFieldText);

    // set text controller
    auto controller = std::make_shared<TextController>();
    myText->setController(controller);

    // create button that will set TextField text from Text
    auto myButton = std::make_shared<Button>(
        "set", ButtonParamsBuilder().onClickHandler([&]() { controller->setText(myTextField->getText()); }));
    column->addChild(myButton);
    column->addChild(myTextField);
    column->addChild(myText);

    // manage the GUI
    std::thread doActions([&]() {
        ASSERT_TRUE(waitForCondition([&]() { return column->isRendered(); }));

        auto myTextFieldObject = myTextField->lvglObject.getObject();
        // clear text field
        char buf1[1] = {'\0'};
        UniqueLock locker(LvglPlus::lvglMutex());
        lv_textarea_set_text(myTextFieldObject, buf1);
        locker.unlock();

        // fill TextField with a new text by emulating of pressing keyboard letters
        for (char letter : newText)
        {
            char buf[] = {letter, 0};
            locker.lock();
            lv_event_send(myTextFieldObject, LV_EVENT_KEY, buf);
            locker.unlock();
        }

        // check myTextField controller updated after changing text
        ASSERT_EQ(myTextField->getText(), newText);

        // emulate myButton clicking
        auto myButtonObject = myButton->lvglObject.getObject();
        locker.lock();
        lv_event_send(myButtonObject, LV_EVENT_CLICKED, nullptr);
        locker.unlock();

        // check myText controller updated after clicking the button
        ASSERT_EQ(myText->getText(), newText);

        closeWindow();
    });

    LvglPlus::setScreen(column);
    LvglPlus::runApp();

    doActions.join();
}

template <typename TextWidget>
static void testTextController(const std::function<const char *(const lv_obj_t *)> &getLvText)
{
    std::string oldText = "oldText";
    std::string newText = "newText";
    auto myController = std::make_shared<TextController>(oldText);
    auto myText = std::make_shared<TextWidget>(myController->getText());
    myText->setController(myController);
    static_cast<Renderable *>(myText.get())->renderWithChildren();
    ASSERT_EQ(myController->getText(), oldText);
    ASSERT_EQ(std::string(getLvText(myText->lvglObject.getObject())), oldText);
    myController->setText(newText);
    ASSERT_EQ(myController->getText(), newText);
    ASSERT_EQ(std::string(getLvText(myText->lvglObject.getObject())), newText);
}

TEST(WidgetTest, TextWithTextController) // Checks Text controlled with TextController
{
    TestParams::getInstance();
    testTextController<Text>(lv_label_get_text);
}

TEST(WidgetTest, TextFieldWithTextController) // Checks TextField controlled with TextController
{
    TestParams::getInstance();
    testTextController<TextField>(lv_textarea_get_text);
}

TEST(WidgetTest, TextFieldWithTextEditingController) // Checks TextField controlled with TextController
{
    TestParams::getInstance();
    std::string oldText = "text";
    char letterToAdd = '1';
    std::string newText = oldText + letterToAdd;
    int initVal = 10;
    const int testVal = 111;
    auto myController = std::make_shared<TextEditingController>(oldText);
    auto myTextField = std::make_shared<TextField>(myController->getText());
    myTextField->setController(myController);
    // set a handler for OnChange event to test it
    myController->setOnChangeHandler([&](const std::string &text) {
        ASSERT_EQ(text, newText);
        initVal = testVal;
    });
    // simulate keyboard button press
    std::thread pressKey([&]() {
        ASSERT_TRUE(waitForRender(myTextField));
        auto myTextFieldObject = myTextField->lvglObject.getObject();
        char buf[] = {letterToAdd, 0};
        UniqueLock locker(LvglPlus::lvglMutex());
        lv_event_send(myTextFieldObject, LV_EVENT_KEY, buf);
        locker.unlock();
        closeWindow();
    });
    LvglPlus::setScreen(myTextField);
    LvglPlus::runApp();
    pressKey.join();
    // check handler was called
    ASSERT_EQ(initVal, testVal);
    // check text field was updated with
    ASSERT_EQ(myController->getText(), newText);
    // check setText doesn't call onChangeHandler
    initVal = 10;
    myController->setText(oldText);
    ASSERT_NE(initVal, testVal);
    ASSERT_EQ(myController->getText(), oldText);
    // check setTextWithNotify calls onChangeHandler
    myController->setTextWithNotify(newText);
    ASSERT_EQ(myController->getText(), newText);
    ASSERT_EQ(initVal, testVal);
}