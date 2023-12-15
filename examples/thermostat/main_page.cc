#include "main_page.hh"
#include "menu_button.hh"
#include "widget_container.hh"
#include "widgets/circular_slider.hh"
#include "widgets/layouts/column.hh"
#include "widgets/layouts/container.hh"
#include "widgets/layouts/row.hh"
#include "widgets/slider.hh"
#include "widgets/text.hh"

using namespace softeq::lvglplus;

const short MainPage::footerWidth = 70;
const short MainPage::timeTextOffset = 10;
const long MainPage::headerColor = 0x14191E;
const short MainPage::headerHeight = 32;
const short MainPage::groupsRadius = 0; // set radius to 0 to make sharp corners
const short MainPage::regulatorFontSize = 40;
const short MainPage::rangeFontSize = 16;
const long MainPage::headerFontColor = 0xffffff;
const long MainPage::footerFontColor = 0x8D97A8;
const long MainPage::rangeFontColor = 0xD2CDC8;
const long MainPage::sliderBackgroundStartColor = 0x606672;
const long MainPage::sliderBackgroundEndColor = 0x33363E;
const long MainPage::footerBackgroundStartColor = 0x202831;
const long MainPage::footerBackgroundEndColor = 0x101215;
const long MainPage::sliderBackgroundColor = 0x0F1215;
const long MainPage::sliderIndicatorColor = 0x3EB0FF;
const long MainPage::sliderTrackColor = 0x9Ba09C;
const short MainPage::grayCircleSize = 145;
const short MainPage::blackCircleSize = 100;
const short MainPage::sliderCircleSize = 175;
const short MainPage::minTemp = 15;
const short MainPage::maxTemp = 35;
const short MainPage::minFanSpeed = 0;
const short MainPage::maxFanSpeed = 4;
const short MainPage::fanSliderWidth = 11;
const short MainPage::fanSliderHeight = 120;

using namespace softeq::streams;

MainPage::MainPage(const std::shared_ptr<StreamController<PageState>> &pageController, Parameters *parameters)
    : Widget("MainPage")
{
    // clang-format off
    auto column = std::make_shared<Column>();
    this->addChild(column);
        // Header
        auto header = std::make_shared<Container>(ContainerParamsBuilder()
                                                    .height(headerHeight)
                                                    .backgroundColor(headerColor)
                                                    .multiChild(true)
                                                    .radius(groupsRadius)); 
        column->addChild(header);
            auto headerDate = std::make_shared<Text>("Friday, March 11, 2022", TextParamsBuilder()
                                                                                .alignment(Alignment::centerLeft)
                                                                                .fontColor(headerFontColor)
                                                                                .offset(timeTextOffset, 0));
            header->addChild(headerDate);
            auto headerTime = std::make_shared<Text>("AM 7:45", TextParamsBuilder()
                                                                    .alignment(Alignment::centerRight)
                                                                    .fontColor(headerFontColor)
                                                                    .offset(-footerWidth - timeTextOffset, 0));
            header->addChild(headerTime);
            auto menuButton = std::make_shared<MenuButton>(ButtonParamsBuilder()
                                                                    .width(footerWidth)
                                                                    .alignment(Alignment::centerRight));
            menuButton->setOnClickHandler([&pageController](){ pageController->add(PageState::SettingsPage); });
            header->addChild(menuButton);

        // Block with a circular slider to set the temperature
        auto regulatorBlock = std::make_shared<Row>();
        column->addChild(regulatorBlock);
            auto sliderGroup = std::make_shared<Container>(ContainerParamsBuilder()
                                                                    .width(LVGL_SCREEN_WIDTH - footerWidth)
                                                                    .height(LVGL_SCREEN_HEIGHT - header->height())
                                                                    .multiChild(true)
                                                                    .backgroundGradient(sliderBackgroundStartColor,
                                                                                        sliderBackgroundEndColor)
                                                                    .radius(groupsRadius));
            regulatorBlock->addChild(sliderGroup);
                // Circular slider to set the temperature
                auto tempRegulator = std::make_shared<CircularSlider>(CircularSliderParamsBuilder()
                                                                                .alignment(Alignment::center)
                                                                                .size(sliderCircleSize,
                                                                                      sliderCircleSize)
                                                                                .value(parameters->temperatureSet)
                                                                                .range(minTemp, maxTemp)
                                                                                .padding(Padding(5))
                                                                                .backgroundColor(sliderBackgroundColor)
                                                                                .indicatorColor(sliderIndicatorColor)
                                                                                .trackColor(sliderTrackColor)
                                                                                .knobColor(sliderIndicatorColor)
                                                                                .backgroundOpacity(0));
                sliderGroup->addChild(tempRegulator);

                // Design element, colored circles for the temperature display
                sliderGroup->addChild(std::make_shared<Container>(ContainerParamsBuilder()
                                                                        .size(grayCircleSize,grayCircleSize)
                                                                        .alignment(Alignment::center)
                                                                        .backgroundGradient(0x646464,0x3C414B)
                                                                        .radius(grayCircleSize)
                                                                        .zIndex(-1)));
                sliderGroup->addChild(std::make_shared<Container>(ContainerParamsBuilder()
                                                                        .size(blackCircleSize,blackCircleSize)
                                                                        .alignment(Alignment::center)
                                                                        .backgroundGradient(0x0C191E,0x191C26)
                                                                        .radius(blackCircleSize)));

                // Text indicators: minimum, maximum, set temperature
                auto tempRangeParams = TextParamsBuilder()
                                            .fontColor(rangeFontColor)
                                            .fontSize(rangeFontSize)
                                            .alignment(Alignment::center);
                // place the text in the left bottom corner
                auto tempMinText = std::make_shared<Text>(std::to_string(minTemp) + "°", tempRangeParams
                                                                    .offset((short)(-(tempRegulator->width())/3),
                                                                            (short)(tempRegulator->height()/2)));
                sliderGroup->addChild(tempMinText);
                // place the text in the right bottom corner
                auto tempMaxText = std::make_shared<Text>(std::to_string(maxTemp) + "°", tempRangeParams
                                                                    .offset((short)(tempRegulator->width()/3),
                                                                            (short)(tempRegulator->height()/2)));
                sliderGroup->addChild(tempMaxText);
                auto tempValueText = std::make_shared<Text>(std::to_string(tempRegulator->value()) + "°",
                                                            TextParamsBuilder()
                                                                    .fontColor(headerFontColor)
                                                                    .fontSize(regulatorFontSize)
                                                                    .alignment(Alignment::center)
                                                                    .offset(2,0)); // calibrate a center alignment

                // Event handler to read temperature value
                tempRegulator->setOnChangeHandler([tempValueText, tempRegulator, parameters]() {
                    parameters->temperatureSet = tempRegulator->value();
                    tempValueText->setText(std::to_string(tempRegulator->value()) + "°");
                });

                // Design elements, dots to indicate quarters of slider
                sliderGroup->addChild(tempValueText);
                auto dotArcParams = TextParamsBuilder().fontSize(regulatorFontSize).alignment(Alignment::center)
                                                                            .fontColor(footerFontColor);
                short regWidth = tempRegulator->width();
                short regHeight = tempRegulator->height();
                // place dots around circular slider
                Offset offsetArcDot[5] = {  Offset((short)(-regWidth/3), (short)(regHeight/3)),
                                            Offset((short)(-regWidth/2), (short)(-regHeight*2/8)),
                                            Offset(0, (short)(-regHeight*3/5)),
                                            Offset((short)(regWidth/2), (short)(-regHeight*2/8)),
                                            Offset((short)(regWidth/3), (short)(regHeight/3))};
                for(auto i : offsetArcDot)
                {
                    sliderGroup->addChild(std::make_shared<Text>(".", dotArcParams.offset(i)));
                }

            // Block with a vertical slider to set a fan speed
            auto footerGroup = std::make_shared<Container>(ContainerParamsBuilder()
                                                                .size(footerWidth, LVGL_SCREEN_HEIGHT - header->height())
                                                                .alignment(Alignment::bottomRight)
                                                                .multiChild(true)
                                                                .backgroundGradient(footerBackgroundStartColor,
                                                                                    footerBackgroundEndColor)
                                                                .radius(groupsRadius));
            regulatorBlock->addChild(footerGroup);
                footerGroup->addChild(std::make_shared<Text>("Fan\nSpeed", TextParamsBuilder()
                                                                                .fontColor(footerFontColor)
                                                                                .alignment(Alignment::topCenter)
                                                                                .offset(0, 25)));
                // A vertical slider to set a fan speed
                auto slider = std::make_shared<Slider>(SliderParamsBuilder()
                                                            .range(minFanSpeed, maxFanSpeed)
                                                            .size(fanSliderWidth, fanSliderHeight)
                                                            .value(parameters->fanSpeed)
                                                            .indicatorColor(sliderIndicatorColor)
                                                            .backgroundColor(sliderBackgroundColor)
                                                            .knobColor(sliderIndicatorColor)
                                                            .alignment(Alignment::center)
                                                            .offset(-12,0));
                slider->setOnChangeHandler([slider, parameters](){ parameters->fanSpeed = slider->value();});
                footerGroup->addChild(slider);
                // Marks with numbers of a fan speed value, place labels with calculated step from the bottom of the
                // slider to the top
                auto sliderMarkParams = TextParamsBuilder().fontColor(footerFontColor).alignment(Alignment::topCenter);
                for(short i = 4; i >= 0; i--)
                {
                    sliderMarkParams.offset(12, (short)(i * slider->height()/4 + 66));
                    footerGroup->addChild(std::make_shared<Text>("-  " + std::to_string(4 - i), sliderMarkParams));
                }
    // clang-format on
}
