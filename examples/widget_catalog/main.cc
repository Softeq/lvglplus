#include "helpers/sdl/sdl_drv.hh"
#include "lvgl_plus.hh"
#include "widgets/button.hh"
#include "widgets/chart.hh"
#include "widgets/checkbox.hh"
#include "widgets/circular_slider.hh"
#include "widgets/divider.hh"
#include "widgets/dropdown_button.hh"
#include "widgets/image.hh"
#include "widgets/layouts/column.hh"
#include "widgets/layouts/container.hh"
#include "widgets/layouts/row.hh"
#include "widgets/led.hh"
#include "widgets/linear_progress_indicator.hh"
#include "widgets/list.hh"
#include "widgets/list_wheel_scroll_view.hh"
#include "widgets/message_box.hh"
#include "widgets/meter.hh"
#include "widgets/properties/symbols.hh"
#include "widgets/properties/theme.hh"
#include "widgets/slider.hh"
#include "widgets/spinbox.hh"
#include "widgets/spinner.hh"
#include "widgets/switch.hh"
#include "widgets/table.hh"
#include "widgets/tabview.hh"
#include "widgets/text.hh"
#include "widgets/text_field.hh"
#include <iostream>
#include <sstream>
#include <thread>

using namespace softeq::lvglplus;

static const short imageZoomMin = 50;
static const short imageZoomMax = 150;
static const short imageZoomStep = 2;
static const short imageZoomInitial = 100;
static const float imageAngleStep = 2;
static const float imageAngleInitial = 0;
static const long lineColor = 0x4f4f4f;
static const short lineHeight = 1;
static const short lineIndentHeight = 20;
static const short exampleContainerSize = 20;
static const long exampleContainerColor[] = {0xff0000, 0x00ff00, 0x0000ff};
static const long exampleTextColor = 0xff0000;
static const long exampleKnobColor = 0x6060ff;
static const short exampleTextWidth = 150;
static const short exampleTextSize = 16;
static const short titlePadding = 10;
static const short exampleButtonWidth = 100;
static const short stateIndent = 8;
static const short sliderIndent = 16;
// set a text width to prevent a row redraw on change
static const short textFieldTextWidth = 80;
static const short linearProgressIndicatorTextWidth = 40;
static const short sliderTextWidth = 50;
static const short listWheelScrollViewTextWidth = 70;
static const short dropDownTextWidth = 65;
static const short switchTextWidth = 70;
static const short checkboxTextWidth = 70;
static const short spinboxTextWidth = 120;
static const short buttonTextWidth = 100;
static const unsigned char spinboxPrecision = 2;
static const int meterMinVal = 20;
static const int meterMaxVal = 40;
static const int meterAngle = 280;
static const int barMinVal = 0;
static const int barMaxVal = 50;
static const int barStep = 2;
static const int chartMinX = 0;
static const int chartMaxX = 10;
static const int chartMinY = 0;
static const int chartMaxY = 100;
static const int chartDemoStep = 10;
static const int chartPoints = 10;
static const int chartSeriesColor1 = 0xff;
static const int chartSeriesColor2 = 0xff00;
static const int chartSeriesColor3 = 0xff0000;
static const float spinboxStep = 0.1;
static const short spinboxDigits = 4;
static const short spinboxFontSize = 16;
static const float spinboxValue = 5.2;
static const long imageStartColor = 0xe0ffff;
static const long imageEndColor = 0xffe0ff;
static const short imageSize = 100;
static const long headerColor = 0xe0e0ff;
static const short headerHeight = 50;
static const short headerFontSize = 18;
static const auto commonRowParams =
    RowParamsBuilder().alignment(Alignment::center).padding(Padding().column(stateIndent));

template <typename T>
static std::string toStringWithPrecision(const T value, const int precision = 2)
{
    std::ostringstream out;
    out.precision(precision);
    out << std::fixed << value;
    return std::move(out).str();
}

static std::shared_ptr<Widget> makeChartExample()
{
    auto rowChart = std::make_shared<Row>(
        RowParamsBuilder().alignment(Alignment::center).padding(Padding(10).column(titlePadding).left(40).bottom(40)));
    auto myChart = std::make_shared<Chart>(ChartParamsBuilder()
                                               .pointCount(chartPoints)
                                               .rangeX(chartMinX, chartMaxX)
                                               .rangeY(chartMinY, chartMaxY)
                                               .addSeries(chartSeriesColor1)
                                               .addSeries(chartSeriesColor2)
                                               .addSeries(chartSeriesColor3));
    rowChart->addChild(myChart);
    std::thread chartDemonstration([myChart]() {
        short val = chartMinY;
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            int i = 0;
            for (auto series : myChart->series())
            {
                series.addValue(val / (i++ + 1));
            }
            val += chartDemoStep;
            if (val > chartMaxY)
            {
                val = chartMinY;
            }
        }
    });
    chartDemonstration.detach();
    return rowChart;
}

static std::shared_ptr<Widget> makeTextFieldExample()
{
    auto rowTextField = std::make_shared<Row>(commonRowParams);
    auto myTextField = std::make_shared<TextField>("text");
    rowTextField->addChild(myTextField);
    auto textFieldText = std::make_shared<Text>(
        myTextField->getText(),
        TextParamsBuilder().width(textFieldTextWidth).alignment(Alignment::center).fontAlign(TextAlignment::left));
    rowTextField->addChild(textFieldText);
    static auto textController = std::make_shared<TextEditingController>(textFieldText->getText());
    myTextField->setController(textController);
    textController->setOnChangeHandler([textFieldText](const std::string &text) { textFieldText->setText(text); });
    return rowTextField;
}

static std::shared_ptr<Widget> makeLedExample()
{
    auto rowLed = std::make_shared<Row>(RowParamsBuilder()
                                            .alignment(Alignment::center)
                                            .padding(Padding(10) // add paddings to draw the LED correctry
                                                         .column(titlePadding)));
    auto myLed = std::make_shared<Led>(LedParamsBuilder().color(0x00ff00));
    rowLed->addChild(myLed);
    std::thread blink([myLed]() {
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            myLed->toggle();
        }
    });
    blink.detach();
    return rowLed;
}

static std::shared_ptr<Widget> makeContainerExample()
{
    auto myContainer = std::make_shared<Container>(ContainerParamsBuilder()
                                                       .alignment(Alignment::center)
                                                       .size(exampleContainerSize, exampleContainerSize)
                                                       .backgroundColor(exampleContainerColor[0]));
    return myContainer;
}

static std::shared_ptr<Widget> makeColumnExample()
{
    auto column = std::make_shared<Column>(ColumnParamsBuilder().alignment(Alignment::center));
    for (long i : exampleContainerColor)
    {
        column->addChild(std::make_shared<Container>(ContainerParamsBuilder()
                                                         .size(exampleContainerSize, exampleContainerSize)
                                                         .alignment(Alignment::center)
                                                         .backgroundColor(i)));
    }
    return column;
}

static std::shared_ptr<Widget> makeRowExample()
{
    auto row = std::make_shared<Row>(RowParamsBuilder().alignment(Alignment::center));
    for (long i : exampleContainerColor)
    {
        row->addChild(std::make_shared<Container>(ContainerParamsBuilder()
                                                      .size(exampleContainerSize, exampleContainerSize)
                                                      .alignment(Alignment::center)
                                                      .backgroundColor(i)));
    }
    return row;
}

static std::shared_ptr<Widget> makeCircularSliderExample()
{
    auto container = std::make_shared<Container>(
        ContainerParamsBuilder().multiChild(true).alignment(Alignment::center).widthEqContent().heightEqContent());

    auto circularSlider = std::make_shared<CircularSlider>(CircularSliderParamsBuilder()
                                                               // add paddings to draw the slider correctry
                                                               .padding(Padding(5))
                                                               .knobColor(exampleKnobColor)
                                                               .alignment(Alignment::center));
    container->addChild(circularSlider);
    auto circularSliderText = std::make_shared<Text>(std::to_string(circularSlider->value()),
                                                     TextParamsBuilder().alignment(Alignment::center));
    container->addChild(circularSliderText);
    circularSlider->setOnChangeHandler([circularSliderText, circularSlider]() {
        circularSliderText->setText(std::to_string(circularSlider->value()));
    });
    return container;
}

static std::shared_ptr<Widget> makeLinearProgressIndicatorExample()
{
    auto rowParams = commonRowParams;
    auto rowLinearProgressIndicator =
        std::make_shared<Row>(rowParams.padding(Padding().top(stateIndent).column(stateIndent)));
    auto bar = std::make_shared<LinearProgressIndicator>();
    bar->setRange(barMinVal, barMaxVal);
    rowLinearProgressIndicator->addChild(bar);
    auto barText = std::make_shared<Text>("", TextParamsBuilder().width(linearProgressIndicatorTextWidth));
    rowLinearProgressIndicator->addChild(barText);
    barText->setText(std::to_string(bar->value()));

    std::thread barDemonstration([bar, barText]() {
        short val = barMinVal;
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
            bar->setValue(val);
            val += barStep;
            if (val > barMaxVal)
            {
                val = barMinVal;
            }
            barText->setText(std::to_string(bar->value()));
        }
    });
    barDemonstration.detach();
    return rowLinearProgressIndicator;
}

static std::shared_ptr<Widget> makeSliderExample()
{
    auto sliderRowParams = commonRowParams;
    auto rowSlider = std::make_shared<Row>(sliderRowParams.padding(Padding(sliderIndent).column(sliderIndent)));
    auto slider = std::make_shared<Slider>();
    rowSlider->addChild(slider);
    auto sliderText = std::make_shared<Text>(
        std::to_string(slider->value()),
        TextParamsBuilder().width(sliderTextWidth).alignment(Alignment::center).fontAlign(TextAlignment::left));
    rowSlider->addChild(sliderText);
    slider->setOnChangeHandler([sliderText, slider]() { sliderText->setText(std::to_string(slider->value())); });
    return rowSlider;
}

static std::shared_ptr<Widget> makeListWheelScrollViewExample()
{
    auto rowParams = commonRowParams;
    auto rowListWheelScrollView =
        std::make_shared<Row>(rowParams.padding(Padding().top(stateIndent).column(stateIndent)));
    auto listWheelScrollView = std::make_shared<ListWheelScrollView>();
    listWheelScrollView->setOptions({"Auto", "Fan", "Heat", "Cool"});
    listWheelScrollView->setIndex(2);
    rowListWheelScrollView->addChild(listWheelScrollView);
    auto listWheelScrollViewToString =
        [](const std::shared_ptr<ListWheelScrollView> &listWheelScrollView) -> std::string {
        return (std::to_string(listWheelScrollView->index()) + ": " + listWheelScrollView->value());
    };
    auto listWheelScrollViewText = std::make_shared<Text>("", TextParamsBuilder()
                                                                  .width(listWheelScrollViewTextWidth)
                                                                  .alignment(Alignment::center)
                                                                  .fontAlign(TextAlignment::left));
    listWheelScrollViewText->setText(listWheelScrollViewToString(listWheelScrollView));
    listWheelScrollView->setOnChangeHandler(
        [listWheelScrollViewText, listWheelScrollView, listWheelScrollViewToString]() {
            listWheelScrollViewText->setText(listWheelScrollViewToString(listWheelScrollView));
        });
    rowListWheelScrollView->addChild(listWheelScrollViewText);
    return rowListWheelScrollView;
}

static std::shared_ptr<Widget> makeDropDownButtonExample()
{
    auto rowParams = commonRowParams;
    auto rowDropdown = std::make_shared<Row>(rowParams.padding(Padding().top(stateIndent).column(stateIndent)));
    auto dropDown = std::make_shared<DropDownButton>(DropDownButtonParamsBuilder().alignment(Alignment::center));
    dropDown->setOptions({"Off", "Slow", "Fast"});
    dropDown->setIndex(2);
    rowDropdown->addChild(dropDown);
    auto dropDownToString = [](const std::shared_ptr<DropDownButton> &dropDown) -> std::string {
        return (std::to_string(dropDown->index()) + ": " + dropDown->value());
    };
    auto dropDownText = std::make_shared<Text>(
        dropDownToString(dropDown),
        TextParamsBuilder().width(dropDownTextWidth).alignment(Alignment::center).fontAlign(TextAlignment::left));
    rowDropdown->addChild(dropDownText);
    dropDown->setOnChangeHandler(
        [dropDownText, dropDown, dropDownToString]() { dropDownText->setText(dropDownToString(dropDown)); });
    return rowDropdown;
}

static std::shared_ptr<Widget> makeSwitchExample()
{
    auto rowSwitch = std::make_shared<Row>(commonRowParams);
    auto mySwitch = std::make_shared<Switch>();
    rowSwitch->addChild(mySwitch);
    auto switchText = std::make_shared<Text>(
        "State: " + std::to_string(mySwitch->state()),
        TextParamsBuilder().width(switchTextWidth).alignment(Alignment::center).fontAlign(TextAlignment::left));
    rowSwitch->addChild(switchText);
    mySwitch->setOnChangeHandler(
        [switchText, mySwitch]() { switchText->setText("State: " + std::to_string(mySwitch->state())); });
    return rowSwitch;
}

static std::shared_ptr<Widget> makeCheckboxExample()
{
    auto rowCheckbox = std::make_shared<Row>(commonRowParams);
    auto myCheckbox = std::make_shared<Checkbox>(CheckboxParamsBuilder().alignment(Alignment::center));
    auto checkboxText = std::make_shared<Text>(
        "", TextParamsBuilder().width(checkboxTextWidth).alignment(Alignment::center).fontAlign(TextAlignment::left));
    myCheckbox->setOnChangeHandler(
        [checkboxText, myCheckbox]() { checkboxText->setText("State: " + std::to_string(myCheckbox->state())); });
    rowCheckbox->addChild(myCheckbox);
    rowCheckbox->addChild(checkboxText);
    checkboxText->setText("State: " + std::to_string(myCheckbox->state()));
    return rowCheckbox;
}

static std::shared_ptr<Widget> makeButtonExample()
{
    auto buttonRowParams = commonRowParams;
    auto rowButton = std::make_shared<Row>(
        buttonRowParams.padding(Padding().column(titlePadding)).alignItems(ChildrenAlignment::center));
    auto colButton = rowButton->addChild(std::make_shared<Column>(ColumnParamsBuilder().padding(Padding().row(10))));
    auto rowParams = RowParamsBuilder()
                         .padding(Padding(5).column(10))
                         .alignment(Alignment::centerRight)
                         .alignItems(ChildrenAlignment::center);
    auto rowDefaultButton = colButton->addChild(std::make_shared<Row>(rowParams));
    auto myButton = std::make_shared<Button>(ButtonParamsBuilder().width(exampleButtonWidth));
    rowDefaultButton->addChild(std::make_shared<Text>("Button"));
    rowDefaultButton->addChild(myButton);
    auto rowTextButton = colButton->addChild(std::make_shared<Row>(rowParams));
    rowTextButton->addChild(std::make_shared<Text>("Text Button"));
    auto myTextButton = std::make_shared<Button>("Click", ButtonParamsBuilder().width(exampleButtonWidth));
    rowTextButton->addChild(myTextButton);
    auto rowImageButton = colButton->addChild(std::make_shared<Row>(rowParams));
    auto imageButton = std::make_shared<Button>("Click", ButtonParamsBuilder()
                                                             .image("/examples/widget_catalog/imgbtn_left.png",
                                                                    "/examples/widget_catalog/imgbtn_mid.png",
                                                                    "/examples/widget_catalog/imgbtn_right.png")
                                                             .width(exampleButtonWidth));
    rowImageButton->addChild(std::make_shared<Text>("Image Button"));
    rowImageButton->addChild(imageButton);
    static int counter = 0;
    auto buttonText =
        std::make_shared<Text>("", TextParamsBuilder().width(buttonTextWidth).fontAlign(TextAlignment::left));
    std::function<void(void)> updateText = [buttonText]() {
        buttonText->setText("Counter: " + std::to_string(counter++));
    };
    updateText();
    rowButton->addChild(buttonText);
    myButton->setOnClickHandler(updateText);
    myTextButton->setOnClickHandler(updateText);
    imageButton->setOnClickHandler(updateText);
    return rowButton;
}

static std::shared_ptr<Widget> makeTextExample()
{
    auto rowText = std::make_shared<Row>(commonRowParams);
    auto myText = std::make_shared<Text>("An example of the Text widget", TextParamsBuilder()
                                                                              .width(exampleTextWidth)
                                                                              .fontAlign(TextAlignment::center)
                                                                              .fontSize(exampleTextSize)
                                                                              .fontColor(exampleTextColor));
    rowText->addChild(myText);
    return rowText;
}

static std::shared_ptr<Widget> makeMeterExample()
{
    auto myMeter = std::make_shared<Meter>(
        MeterParamsBuilder().alignment(Alignment::center).angle(meterAngle).range(meterMinVal, meterMaxVal));
    std::thread meterDemonstration([myMeter]() {
        short val = meterMinVal;
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            myMeter->setValue(val);
            val++;
            if (val > meterMaxVal)
            {
                val = meterMinVal;
            }
        }
    });
    meterDemonstration.detach();
    return myMeter;
}

static std::shared_ptr<Widget> makeSpinboxExample()
{
    // add paddings to draw slider correctry
    auto rowSpinbox =
        std::make_shared<Row>(RowParamsBuilder().alignment(Alignment::center).padding(Padding().column(stateIndent)));
    auto spinbox = std::make_shared<Spinbox>(SpinboxParamsBuilder()
                                                 .alignment(Alignment::center)
                                                 .fontSize(spinboxFontSize)
                                                 .digitFormat(spinboxDigits, spinboxPrecision)
                                                 .step(spinboxStep));
    rowSpinbox->addChild(spinbox);
    spinbox->setValue(spinboxValue);
    spinbox->increment();
    auto spinboxText = std::make_shared<Text>(
        "", TextParamsBuilder().width(spinboxTextWidth).alignment(Alignment::center).fontAlign(TextAlignment::left));
    rowSpinbox->addChild(spinboxText);
    spinbox->setOnChangeHandler([spinboxText, spinbox]() {
        spinboxText->setText("Value: " + toStringWithPrecision(spinbox->value(), spinboxPrecision));
    });
    spinboxText->setText("Value: " + toStringWithPrecision(spinbox->value(), spinboxPrecision));
    return rowSpinbox;
}

static std::shared_ptr<Widget> makeSpinnerExample()
{
    auto spinner = std::make_shared<Spinner>(SpinnerParamsBuilder().alignment(Alignment::center));
    return spinner;
}

static std::shared_ptr<Widget> makeTableExample()
{
    auto table = std::make_shared<Table>(TableParamsBuilder().alignment(Alignment::center));
    table->setTitles({"Name", "Price"});
    table->addValues({"Apple", "$1"});
    table->addValues({"Banana", "$2"});
    table->addValues({"Lemon", "$3"});
    table->setValue(3, 1, "$4"); // set value for 3rd row/1st column of "table".
    return table;
}

static std::shared_ptr<Widget> makeTabviewExample()
{
    auto container =
        std::make_shared<Container>(ContainerParamsBuilder().padding(Padding().top(stateIndent)).heightEqContent());
    auto tabview = std::make_shared<Tabview>(TabviewParamsBuilder().alignment(Alignment::center));
    container->addChild(tabview);
    auto content3 = std::make_shared<Container>(ContainerParamsBuilder().backgroundColor(0x50ff50));
    content3->addChild(std::make_shared<Text>("Content 3", TextParamsBuilder().alignment(Alignment::center)));
    tabview->addChild("Tab 1", std::make_shared<Text>("Content 1"));
    tabview->addChild("Tab 2",
                      std::make_shared<Button>(
                          "To Tab 3", ButtonParamsBuilder().alignment(Alignment::center).onClickHandler([tabview]() {
                              tabview->openTab(2);
                          })));
    tabview->addChild("Tab 3", content3);
    return container;
}

static std::shared_ptr<Widget> makeDividerExample()
{
    auto row = std::make_shared<Row>(RowParamsBuilder()
                                         .alignment(Alignment::center)
                                         .alignItems(ChildrenAlignment::center)
                                         .padding(Padding().column(20)));
    auto columnD = row->addChild(std::make_shared<Column>());
    auto textParams = TextParamsBuilder().alignment(Alignment::center);
    columnD->addChild(std::make_shared<Text>("Column", textParams));
    columnD->addChild(std::make_shared<Divider>());
    columnD->addChild(std::make_shared<Text>("divider", textParams));

    auto rowD = row->addChild(std::make_shared<Row>());
    rowD->addChild(std::make_shared<Text>("Row", textParams));
    rowD->addChild(std::make_shared<Divider>());
    rowD->addChild(std::make_shared<Text>("divider", textParams));
    return row;
}

static std::shared_ptr<Widget> makeListExample()
{
    auto list = std::make_shared<List>(ListParamsBuilder().alignment(Alignment::center));
    list->addText("File");
    list->addButton(Symbols::FILE + " New");
    list->addButton(Symbols::DIRECTORY + " Open");
    list->addButton(Symbols::SAVE + " Save");
    list->addText("Connectivity");
    list->addButton(Symbols::BLUETOOTH + " Bluetooth");
    list->addButton(Symbols::GPS + " Navigation");
    return list;
}

static std::shared_ptr<Widget> makeMessageBoxExample()
{
    auto mainColumn = std::make_shared<Column>(
        ColumnParamsBuilder().alignContent(ChildrenAlignment::center).alignItems(ChildrenAlignment::center));
    mainColumn->setPadding(Padding(5).row(10));

    auto mbText = std::make_shared<Text>("Call the message box", TextParamsBuilder().alignment(Alignment::center));
    mainColumn->addChild(mbText);
    auto mbButton = std::make_shared<Button>("Message box");
    mbButton->setOnClickHandler([mbText]() {
        LvglPlus::messageBox(mbText, MessageBoxParamsBuilder()
                                         .title("Message")
                                         .text("Text in the message box.")
                                         .addButton("OK", ButtonParamsBuilder().onClickHandler(
                                                              [mbText]() { mbText->setText("OK button clicked"); }))
                                         .onClose([mbText]() { mbText->setText("Close button clicked"); }));
    });
    mainColumn->addChild(mbButton);
    return mainColumn;
}

static std::shared_ptr<Widget> makeImageExample()
{
    auto container = std::make_shared<Container>(ContainerParamsBuilder()
                                                     .size(imageSize, imageSize)
                                                     .backgroundGradient(imageStartColor, imageEndColor)
                                                     .alignment(Alignment::center));
    auto myImage = std::make_shared<Image>();
    myImage->setImage("/examples/widget_catalog/img_cogwheel_argb.png");
    std::thread imageDemonstration([myImage]() {
        bool zoomDirUp = true;
        float angle = imageAngleInitial;
        short zoom = imageZoomInitial;
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            myImage->setZoom(zoom);
            myImage->setAngle(angle);
            angle += imageAngleStep;
            if (angle >= 360) // 360 degrees = 0 degrees
            {
                angle = 0;
            }
            if (zoomDirUp)
            {
                zoom += imageZoomStep;
                if (zoom >= imageZoomMax)
                {
                    zoomDirUp = false;
                }
            }
            else
            {
                zoom -= imageZoomStep;
                if (zoom <= imageZoomMin)
                {
                    zoomDirUp = true;
                }
            }
        }
    });
    imageDemonstration.detach();
    container->addChild(myImage);
    return container;
}

unsigned long colors[] = {0x2020e0, 0x20a020, 0xe02020};
unsigned long layoutColors[] = {0x20a020, 0x902020, 0x2020e0};
int colorCnt = 0;
static std::shared_ptr<Widget> makeThemeExample()
{
    auto myRow = std::make_shared<Row>(RowParamsBuilder().alignment(Alignment::center).padding(Padding(10).column(10)));
    auto root = myRow->root();
    myRow->addChild(std::make_shared<Button>("Theme 1", ButtonParamsBuilder().onClickHandler([root]() {
        LvglPlus::setCustomTheme(
            root,
            std::make_shared<Theme>(
                ThemeBuilder().backgroundColor(0x80ff80).mainColor(0x202080).layoutPadding(Padding(0)).textColor(0)));
    })));
    myRow->addChild(std::make_shared<Button>("Theme 2", ButtonParamsBuilder().onClickHandler([root]() {
        LvglPlus::setCustomTheme(root, std::make_shared<Theme>(ThemeBuilder()
                                                                   .backgroundColor(0)
                                                                   .mainColor(0x009000)
                                                                   .layoutPadding(Padding(0))
                                                                   .textColor(0xa0a0a0)
                                                                   .knobColor(0xff9090)
                                                                   .inactiveColor(0x505050)
                                                                   .trackColor(0x70e070)));
    })));
    myRow->addChild(std::make_shared<Button>(
        "No theme", ButtonParamsBuilder().onClickHandler([root]() { LvglPlus::removeCustomTheme(root); })));
    return myRow;
}

static void addExample(const std::shared_ptr<Widget> &parent, const std::string &title,
                       const std::function<std::shared_ptr<Widget>(void)> &example)
{
    parent->addChild(std::make_shared<Text>(title, TextParamsBuilder().padding(Padding().left(titlePadding))));
    parent->addChild(example());
    parent->addChild(std::make_shared<Divider>(DividerParamsBuilder().lineIndent(35)));
}

static std::shared_ptr<Widget> makeExamples()
{
    auto mainColumn = std::make_shared<Column>(ColumnParamsBuilder().heightPctOfParent(100));

    auto titleContainer = mainColumn->addChild(
        std::make_shared<Container>(ContainerParamsBuilder().height(headerHeight).backgroundColor(headerColor)));
    titleContainer->addChild(std::make_shared<Text>(
        "Widget catalog", TextParamsBuilder().alignment(Alignment::center).fontSize(headerFontSize).fontColor(0)));
    mainColumn->addChild(std::make_shared<Divider>(DividerParamsBuilder().lineIndent(1)));
    mainColumn->addChild(std::make_shared<Container>(ContainerParamsBuilder().height(15)));

    addExample(mainColumn, "Container", makeContainerExample);
    addExample(mainColumn, "Column", makeColumnExample);
    addExample(mainColumn, "Row", makeRowExample);
    addExample(mainColumn, "Divider", makeDividerExample);
    addExample(mainColumn, "Text", makeTextExample);
    addExample(mainColumn, "TextField", makeTextFieldExample);
    addExample(mainColumn, "Button", makeButtonExample);
    addExample(mainColumn, "Checkbox", makeCheckboxExample);
    addExample(mainColumn, "Switch", makeSwitchExample);
    addExample(mainColumn, "Dropdown button", makeDropDownButtonExample);
    addExample(mainColumn, "List wheel scroll view", makeListWheelScrollViewExample);
    addExample(mainColumn, "Slider", makeSliderExample);
    addExample(mainColumn, "Circular slider", makeCircularSliderExample);
    addExample(mainColumn, "Linear progress indicator", makeLinearProgressIndicatorExample);
    addExample(mainColumn, "List", makeListExample);
    addExample(mainColumn, "Tabview", makeTabviewExample);
    addExample(mainColumn, "Table", makeTableExample);
    addExample(mainColumn, "Spinner", makeSpinnerExample);
    addExample(mainColumn, "Spinbox", makeSpinboxExample);
    addExample(mainColumn, "Meter", makeMeterExample);
    addExample(mainColumn, "Led", makeLedExample);
    addExample(mainColumn, "Chart", makeChartExample);
    addExample(mainColumn, "Image", makeImageExample);
    addExample(mainColumn, "Theme", makeThemeExample);
    addExample(mainColumn, "Message box", makeMessageBoxExample);

    return mainColumn;
}

int main(int argc, char *argv[])
{
    LvglPlus::initialize();
    addSdlPeripherals(LVGL_SCREEN_WIDTH, LVGL_SCREEN_HEIGHT);
    auto screen = makeExamples();
    LvglPlus::setScreen(screen);
    LvglPlus::runApp();
    return 0;
}
