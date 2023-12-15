#ifndef LVGL_SETTINGS_PAGE_HH
#define LVGL_SETTINGS_PAGE_HH

#include "thermostat.hh"
#include "widget.hh"
#include <communication/streams/include/stream_controller.hh>

namespace slvglplus = softeq::lvglplus;
namespace sstreams = softeq::streams;

class SettingsPage : public slvglplus::Widget
{
public:
    SettingsPage(const std::shared_ptr<sstreams::StreamController<PageState>> &pageController, Parameters *parameters);

private:
    static const short headerHeight;
    static const long headerColor;
    static const short backButtonWidth;
    static const short groupsRadius;
    static const long settingsBackgroundStartColor;
    static const long settingsBackgroundEndColor;
    static const long fontColor;
    static const long rollerBackgroundStartColor;
    static const long rollerBackgroundEndColor;
    static const long lineColor;
    static const short oneLineParamHeight;
    static const short twoLineParamHeight;
    static const long checkboxFontColor;
    static const long styleColor;
    static const short checkboxGroupWidth;
    static const long switchBackgroundColor;
    static const short checkboxTextIndent;
    static const short groupIndent;
    static const long dropdownBackgroundColor;
    static const long dropdownFontColor;
    static const short switchWidth;
    static const short lineHeight;
    static const short checkboxBorderWidth;
};

#endif // LVGL_SETTINGS_PAGE_HH
