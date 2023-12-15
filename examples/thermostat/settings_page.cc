#include "settings_page.hh"
#include "widget_container.hh"
#include "widgets/button.hh"
#include "widgets/checkbox.hh"
#include "widgets/divider.hh"
#include "widgets/dropdown_button.hh"
#include "widgets/layouts/column.hh"
#include "widgets/list_wheel_scroll_view.hh"
#include "widgets/switch.hh"
#include "widgets/text.hh"

using namespace softeq::lvglplus;

const short SettingsPage::headerHeight = 41;
const short SettingsPage::backButtonWidth = 61;
const long SettingsPage::styleColor = 0x35B5F0;
const long SettingsPage::headerColor = 0x14191E;
const long SettingsPage::fontColor = 0xffffff;
const long SettingsPage::lineColor = 0x4f4f4f;
const short SettingsPage::groupsRadius = 0; // set radius to 0 to make sharp corners
const long SettingsPage::settingsBackgroundStartColor = 0x282C33;
const long SettingsPage::settingsBackgroundEndColor = 0x121518;
const long SettingsPage::rollerBackgroundStartColor = 0x626363;
const long SettingsPage::rollerBackgroundEndColor = 0x3D424B;
const long SettingsPage::switchBackgroundColor = 0x5B606C;
const long SettingsPage::dropdownBackgroundColor = 0xffffff;
const long SettingsPage::dropdownFontColor = 0;
const short SettingsPage::oneLineParamHeight = 50;
const short SettingsPage::twoLineParamHeight = 120;
const long SettingsPage::checkboxFontColor = 0xA8B0BB;
const short SettingsPage::checkboxGroupWidth = 130;
const short SettingsPage::checkboxTextIndent = 5;
const short SettingsPage::checkboxBorderWidth = 0;
const short SettingsPage::groupIndent = 10;
const short SettingsPage::switchWidth = 50;
const short SettingsPage::lineHeight = 1;

using namespace softeq::streams;

SettingsPage::SettingsPage(const std::shared_ptr<StreamController<PageState>> &pageController, Parameters *parameters)
    : Widget("SettingsPage")
{
    setBorderColor(0xffff);
    // clang-format off
    auto column = this -> addChild(std::make_shared<Column>());
        // Header
        auto header = std::make_shared<Container>(ContainerParamsBuilder()
                                                    .height(headerHeight)
                                                    .backgroundColor(headerColor)
                                                    .multiChild(true)
                                                    .radius(groupsRadius)
                                                    .padding(Padding().left(groupIndent).right(groupIndent)));
        column->addChild(header);
            // A button to return to the main page
            auto backButton = std::make_shared<Button>("Back", ButtonParamsBuilder()
                                                                    .backgroundColor(styleColor)
                                                                    .size(backButtonWidth, headerHeight)
                                                                    .fontColor(fontColor));
            // Event handler on click - send a main page state to the streamBuilder
            backButton->setOnClickHandler([&pageController](){ pageController->add(PageState::MainPage); });
            header->addChild(backButton);
            auto headerLabel = std::make_shared<Text>("Settings", TextParamsBuilder()
                                                                    .alignment(Alignment::centerRight)
                                                                    .fontColor(fontColor));
            header->addChild(headerLabel);
        // A block with settings
        auto settingsBlock = std::make_shared<Column>(ColumnParamsBuilder()
                                                                .width(LVGL_SCREEN_WIDTH)
                                                                .height(LVGL_SCREEN_HEIGHT - header->height())
                                                                .backgroundGradient(settingsBackgroundStartColor,
                                                                                    settingsBackgroundEndColor)
                                                                .radius(groupsRadius));
            auto settingsTextStyle = TextParamsBuilder()
                                        .fontColor(fontColor).alignment(Alignment::centerLeft);
            auto groupStyle = ContainerParamsBuilder().width(LVGL_SCREEN_WIDTH).height(oneLineParamHeight)
                                                                        .padding(Padding(groupIndent)).multiChild(true);
            auto switchStyle = SwitchParamsBuilder().width(switchWidth).alignment(Alignment::centerRight)
                    .backgroundColor(switchBackgroundColor).indicatorColor(styleColor).borderWidth(checkboxBorderWidth);

            auto quietGroup = std::make_shared<Container>(groupStyle);
                quietGroup->addChild(std::make_shared<Text>("Quiet", settingsTextStyle));
                auto quietSwitch = std::make_shared<Switch>(switchStyle);
                quietSwitch->setState(parameters->quietEnable);
                quietSwitch->setOnChangeHandler([parameters, quietSwitch]()
                                                        {parameters->quietEnable = quietSwitch->state();});
                quietGroup->addChild(quietSwitch);
            settingsBlock->addChild(quietGroup);
            settingsBlock->addChild(std::make_shared<Divider>());

            auto turboGroup = std::make_shared<Container>(groupStyle);
                turboGroup->addChild(std::make_shared<Text>("Turbo", settingsTextStyle));
                auto turboSwitch = std::make_shared<Switch>(switchStyle);
                turboSwitch->setState(parameters->turboEnable);
                turboSwitch->setOnChangeHandler([parameters, turboSwitch]()
                                                        {parameters->turboEnable = turboSwitch->state();});
                turboGroup->addChild(turboSwitch);
            settingsBlock->addChild(turboGroup);
            settingsBlock->addChild(std::make_shared<Divider>());

            auto fanSwingGroup = std::make_shared<Container>(groupStyle);
                fanSwingGroup->addChild(std::make_shared<Text>("Fan swing", settingsTextStyle));
                auto dropDown = std::make_shared<DropDownButton>(DropDownButtonParamsBuilder()
                                                                            // make less space around the text
                                                                            .padding(Padding(5))
                                                                            .alignment(Alignment::centerRight)
                                                                            .backgroundColor(dropdownBackgroundColor)
                                                                            .fontColor(dropdownFontColor));
                dropDown->setOptions({"Off", "Slow", "Fast"});
                dropDown->setIndex(parameters->fanSwing);
                dropDown->setOnChangeHandler([parameters, dropDown](){ parameters->fanSwing = dropDown->index(); });
                fanSwingGroup->addChild(dropDown);
            settingsBlock->addChild(fanSwingGroup);
            settingsBlock->addChild(std::make_shared<Divider>());

            auto zoneGroup = std::make_shared<Container>(groupStyle.height(twoLineParamHeight));
                zoneGroup->addChild(std::make_shared<Text>("Zone", settingsTextStyle));
                auto checkboxStyle = CheckboxParamsBuilder()
                                                    .fontColor(checkboxFontColor)
                                                    .indicatorColor(styleColor)
                                                    .padding(Padding().column(checkboxTextIndent))
                                                    .borderWidth(checkboxBorderWidth)
                                                    .inactiveColor(switchBackgroundColor);
                auto checkboxGroup = std::make_shared<Column>(ColumnParamsBuilder()
                                                                    .padding(Padding(groupIndent).row(groupIndent))
                                                                    .width(checkboxGroupWidth)
                                                                    .alignment(Alignment::centerRight));
                auto bedroomCheckbox = std::make_shared<Checkbox>(checkboxStyle.text("Bedroom"));
                bedroomCheckbox->setState(parameters->bedroomEnable);
                bedroomCheckbox->setOnChangeHandler([parameters, bedroomCheckbox]()
                                                { parameters->bedroomEnable = bedroomCheckbox->state(); });
                auto kitchenCheckbox = std::make_shared<Checkbox>(checkboxStyle.text("Kitchen"));
                kitchenCheckbox->setState(parameters->kitchenEnable);
                kitchenCheckbox->setOnChangeHandler([parameters, kitchenCheckbox]()
                                                { parameters->kitchenEnable = kitchenCheckbox->state(); });
                auto livingroomCheckbox = std::make_shared<Checkbox>(checkboxStyle.text("Livingroom"));
                livingroomCheckbox->setState(parameters->livingroomEnable);
                livingroomCheckbox->setOnChangeHandler([parameters, livingroomCheckbox]()
                                                { parameters->livingroomEnable = livingroomCheckbox->state(); });
                checkboxGroup->addChild(bedroomCheckbox);
                checkboxGroup->addChild(kitchenCheckbox);
                checkboxGroup->addChild(livingroomCheckbox);
                zoneGroup->addChild(checkboxGroup);
            settingsBlock->addChild(zoneGroup);
            settingsBlock->addChild(std::make_shared<Divider>());

            auto modeGroup = std::make_shared<Column>(ColumnParamsBuilder()
                                                                    .padding(Padding(groupIndent).row(groupIndent)));
                modeGroup->addChild(std::make_shared<Text>("Mode selection", settingsTextStyle
                                                                                        .alignment(Alignment::center)));
                auto roller = std::make_shared<ListWheelScrollView>(ListWheelScrollViewParamsBuilder()
                                                                .borderColor(lineColor)
                                                                .indicatorColor(styleColor)
                                                                .alignment(Alignment::center)
                                                                .backgroundGradient(rollerBackgroundStartColor,
                                                                                    rollerBackgroundEndColor));
                roller->setOptions({"Auto", "Cool", "Heat", "Fan"});
                roller->setIndex(parameters->mode);
                roller->setOnChangeHandler([parameters, roller](){ parameters->mode = roller->index(); });
                modeGroup->addChild(roller);
            settingsBlock->addChild(modeGroup);
            settingsBlock->addChild(std::make_shared<Divider>());
        column->addChild(settingsBlock);
    // clang-format on
}
