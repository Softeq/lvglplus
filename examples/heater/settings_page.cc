#include "settings_page.hh"
#include "controllers/text_editing_controller.hh"
#include "widget_container.hh"
#include "widgets/button.hh"
#include "widgets/layouts/column.hh"
#include "widgets/layouts/row.hh"
#include "widgets/text.hh"
#include "widgets/text_field.hh"

using namespace softeq::lvglplus;

const std::string SettingsPage::defaultAdjInTemperature = "25.2";
const short SettingsPage::colInTempBoxWidth = 350;
const short SettingsPage::titlePadding = 20;
const short SettingsPage::tempBoxBorderWidth = 5;
const short SettingsPage::tempBoxPadding = 10;
const long SettingsPage::inTempBoxColor = 0xA0EFFF;
const short SettingsPage::tempTextWidth = 50;
const short SettingsPage::indentBetweenTempBoxAndButton = 20;
const short SettingsPage::indentBetweenTexts = 5;
const short SettingsPage::indentBetweenTempRows = 5;
const short SettingsPage::buttonSetWidth = 55;
const short SettingsPage::buttonSetHeight = 30;
const short SettingsPage::backButtonWidth = 80;

SettingsPage::SettingsPage(const std::shared_ptr<NavigationBloc> &navigationBloc, const std::string &pageName,
                           const std::shared_ptr<slvglplus::TextEditingController> &insideTempCal)
    : Widget("SettingsPage")
{
    // clang-format off
    auto column = this -> addChild(std::make_shared<Column>(ColumnParamsBuilder()
                                                                .alignItems(ChildrenAlignment::center)
                                                                .alignContent(ChildrenAlignment::center)
                                                                .padding(Padding().bottom(5))));
        column -> addChild(std::make_shared<Text>(pageName, TextParamsBuilder()
                                                    .padding(Padding(titlePadding))));
        // box with temperature values
        auto colInTemp = column -> addChild(std::make_shared<Column>(ColumnParamsBuilder()
                                                                            .width(colInTempBoxWidth)
                                                                            .borderWidth(tempBoxBorderWidth)
                                                                            .padding(Padding(tempBoxPadding))
                                                                            .backgroundColor(inTempBoxColor)
                                                                            .alignItems(ChildrenAlignment::center)
                                                                            ));
            // row with adjusted temperature editor and set button
            auto rowInEditTempAdj = colInTemp -> addChild(std::make_shared<Row>(RowParamsBuilder()
                                                                            .alignContent(ChildrenAlignment::center)
                                                                            .alignItems(ChildrenAlignment::center)
                                                                            .padding(Padding().column(5))
                                                                            ));
                rowInEditTempAdj -> addChild(std::make_shared<Text>("Temperature calibration: "));
                // create controller to save adjusted temperature
                auto editorTempCal = std::make_shared<TextEditingController>(defaultAdjInTemperature);
                // create TextField to enter temperature
                auto calibrationInsideTextField = std::make_shared<TextField>(editorTempCal->getText(),
                                                                    TextFieldParamsBuilder().width(tempTextWidth));
                calibrationInsideTextField->setController(editorTempCal);
                rowInEditTempAdj -> addChild(calibrationInsideTextField);
                // indent
                rowInEditTempAdj -> addChild(std::make_shared<Row>(RowParamsBuilder().width(indentBetweenTexts)));
                // create button to get adjusted temperature, update linked label and send it to user handler
                rowInEditTempAdj -> addChild(std::make_shared<Button>("Set", ButtonParamsBuilder()
                            .height(buttonSetHeight)
                            .width(buttonSetWidth)
                            .onClickHandler([insideTempCal, editorTempCal]()
                                {
                                    insideTempCal->setTextWithNotify(editorTempCal->getText());
                                })));
            // indent
            colInTemp -> addChild(std::make_shared<Column>(ColumnParamsBuilder().height(indentBetweenTempRows)));
            // row with adjusted temperature display
            auto rowInTempAdj = colInTemp -> addChild(std::make_shared<Row>());
                rowInTempAdj -> addChild(std::make_shared<Text>("Current setting: "));
                // text with adjusted temperature, auto update with controller
                auto insideAdjTempText = std::make_shared<Text>(insideTempCal->getText(), TextParamsBuilder()
                                                                                    .width(tempTextWidth)
                                                                                    .fontAlign(TextAlignment::center));
                insideAdjTempText->setController(insideTempCal);
                rowInTempAdj -> addChild(insideAdjTempText);
                rowInTempAdj -> addChild(std::make_shared<Text>("°C"));
        // indent
        column -> addChild(std::make_shared<Row>(RowParamsBuilder().height(indentBetweenTempBoxAndButton)));
        // go back button
        column -> addChild(std::make_shared<Button>("Back", ButtonParamsBuilder()
                                                                .width(backButtonWidth)
                                                                .onClickHandler([navigationBloc]()
                                                                {
                                                                    navigationBloc->add(OpenMainPageEvent());
                                                                })));
    // clang-format on
}
