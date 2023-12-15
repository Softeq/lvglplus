#ifndef LVGL_MAIN_PAGE_HH
#define LVGL_MAIN_PAGE_HH

#include "thermostat.hh"
#include "widget.hh"
#include <communication/streams/include/stream_controller.hh>

namespace slvglplus = softeq::lvglplus;
namespace sstreams = softeq::streams;

class MainPage : public slvglplus::Widget
{
public:
    MainPage(const std::shared_ptr<sstreams::StreamController<PageState>> &pageController, Parameters *parameters);

private:
    static const short footerWidth;
    static const short timeTextOffset;
    static const long headerColor;
    static const short headerHeight;
    static const short groupsRadius;
    static const long headerFontColor;
    static const long sliderBackgroundStartColor;
    static const long sliderBackgroundEndColor;
    static const long sliderBackgroundColor;
    static const long sliderIndicatorColor;
    static const long sliderTrackColor;
    static const short grayCircleSize;
    static const short blackCircleSize;
    static const short sliderCircleSize;
    static const short minTemp;
    static const short maxTemp;
    static const long footerFontColor;
    static const short regulatorFontSize;
    static const short rangeFontSize;
    static const long rangeFontColor;
    static const short minFanSpeed;
    static const short maxFanSpeed;
    static const short fanSliderWidth;
    static const short fanSliderHeight;
    static const long footerBackgroundStartColor;
    static const long footerBackgroundEndColor;
};

#endif // LVGL_MAIN_PAGE_HH
