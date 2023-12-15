#include "helpers/sdl/sdl_drv.hh"
#include "lvgl_plus.hh"
#include "widgets/layouts/column.hh"
#include "widgets/layouts/container.hh"
#include "widgets/text_field.hh"

using namespace softeq::lvglplus;
static const auto columnParams =
    ColumnParamsBuilder().alignment(Alignment::center).widthEqContent().padding(Padding(10).row(10));

static std::shared_ptr<Widget> makeScreen(int i)
{
    auto myColumn = std::make_shared<Column>(columnParams);
    std::string screenNumber = "Screen " + std::to_string(i);
    myColumn->addChild(std::make_shared<Text>(screenNumber));
    myColumn->addChild(std::make_shared<TextField>(""));
    auto root = myColumn->root();
    myColumn->addChild(std::make_shared<Button>("Message", ButtonParamsBuilder().onClickHandler([root, screenNumber]() {
        LvglPlus::messageBox(root, MessageBoxParamsBuilder().title(screenNumber));
    })));
    return myColumn;
}

int main(int argc, char *argv[])
{
    LvglPlus::initialize();
    for (int i = 0; i < 4; i++)
    {
        addSdlPeripherals(300 + i * 50, 200 + i * 50); // set different windows sizes
        LvglPlus::setScreen(makeScreen(i), LvglPlus::findDisplay(i));
    }
    LvglPlus::runApp();
    return 0;
}