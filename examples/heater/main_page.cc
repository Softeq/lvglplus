#include "main_page.hh"
#include "widget_container.hh"
#include "widgets/button.hh"
#include "widgets/layouts/column.hh"
#include "widgets/layouts/row.hh"
#include "widgets/text.hh"

using namespace softeq::lvglplus;

const short MainPage::colInTempBoxWidth = 350;
const short MainPage::titlePadding = 20;
const short MainPage::tempBoxBorderWidth = 5;
const short MainPage::tempBoxPadding = 10;
const long MainPage::inTempBoxColor = 0xA0EFFF;
const long MainPage::outTempBoxColor = 0xB0DFFF;
const short MainPage::tempTextWidth = 50;
const short MainPage::settingsButtonWidth = 80;
const short MainPage::indentBetweenTempBoxes = 10;
const short MainPage::indentBetweenTempBoxAndButton = 20;

MainPage::MainPage(const std::shared_ptr<NavigationBloc> &navigationBloc, const std::string &pageName,
                   const std::shared_ptr<TextEditingController> &insideTemp,
                   const std::shared_ptr<TextEditingController> &outsideTemp,
                   const std::shared_ptr<TextEditingController> &insideTempCal)
    : Widget("MainPage")
{
    // clang-format off
    auto column = this->addChild(std::make_shared<Column>(ColumnParamsBuilder()
                                                            .alignItems(ChildrenAlignment::center)
                                                            .alignContent(ChildrenAlignment::center)
                                                            .padding(Padding().bottom(5))
                                                            ));
        column -> setDiagnosticName("MainPage main column");
        column -> addChild(std::make_shared<Text>(pageName, TextParamsBuilder()
                                                        .padding(Padding(titlePadding))
                                                 ));
        // box with current and adjusted inside temperature
        auto colInTemp = column -> addChild(std::make_shared<Column>(ColumnParamsBuilder()
                                                                        .width(colInTempBoxWidth)
                                                                        .alignItems(ChildrenAlignment::center)
                                                                        .alignContent(ChildrenAlignment::center)
                                                                        .borderWidth(tempBoxBorderWidth)
                                                                        .padding(Padding(tempBoxPadding))
                                                                        .backgroundColor(inTempBoxColor)
                                                                    ));
            // row with current inside temperature
            auto rowInTemp = colInTemp -> addChild(std::make_shared<Row>());
                rowInTemp -> addChild(std::make_shared<Text>("Current inside temperature:"));
                // create Text to show current temperature, auto update with controller
                auto insideTempText = std::make_shared<Text>(insideTemp->getText(), TextParamsBuilder()
                                                                                    .width(tempTextWidth)
                                                                                    .fontAlign(TextAlignment::center));
                insideTempText->setController(insideTemp);
                rowInTemp -> addChild(insideTempText);
                rowInTemp -> addChild(std::make_shared<Text>("°C"));
            // row with adjusted inside temperature
            auto rowInTempAdj = colInTemp -> addChild(std::make_shared<Row>(RowParamsBuilder()
                                                                                .padding(Padding().top(5))
                                                                                ));
                rowInTempAdj -> addChild(std::make_shared<Text>("Inside temperature calibration:"));
                // create Text to show adjusted temperature, auto update with controller
                auto insideAdjTempText = std::make_shared<Text>(insideTempCal->getText(), TextParamsBuilder()
                                                                                    .width(tempTextWidth)
                                                                                    .fontAlign(TextAlignment::center));
                insideAdjTempText->setController(insideTempCal);
                rowInTempAdj -> addChild(insideAdjTempText);
                rowInTempAdj -> addChild(std::make_shared<Text>("°C"));
        // indent
        column -> addChild(std::make_shared<Row>(RowParamsBuilder().height(indentBetweenTempBoxes)));
        // box with current outside temperature
        auto rowOutTemp = column -> addChild(std::make_shared<Row>(RowParamsBuilder()
                                                                    .borderWidth(tempBoxBorderWidth)
                                                                    .padding(Padding(tempBoxPadding))
                                                                    .backgroundColor(outTempBoxColor)
                                                                    ));
            rowOutTemp -> addChild(std::make_shared<Text>("Current outside temperature:"));
            // create Text for outside temperature, auto update with controller
            auto outsideTempText = std::make_shared<Text>(outsideTemp->getText(), TextParamsBuilder()
                                                                                    .width(tempTextWidth)
                                                                                    .fontAlign(TextAlignment::center));
            outsideTempText->setController(outsideTemp);
            rowOutTemp -> addChild(outsideTempText);
            rowOutTemp -> addChild(std::make_shared<Text>("°C"));
        // indent
        column -> addChild(std::make_shared<Row>(RowParamsBuilder().height(indentBetweenTempBoxAndButton)));
        // button to open settings page
        column -> addChild(std::make_shared<Button>("Settings", ButtonParamsBuilder()
                                                                    .width(settingsButtonWidth)
                                                                    .onClickHandler([navigationBloc]()
                                                                    {
                                                                        navigationBloc->add(OpenSettingsPageEvent());
                                                                    })));
    // clang-format on
}
