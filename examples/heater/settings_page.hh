#ifndef LVGL_SETTINGS_PAGE_HH
#define LVGL_SETTINGS_PAGE_HH

#include "controllers/text_editing_controller.hh"
#include "navigation_bloc/navigation_bloc.hh"
#include "widget.hh"

namespace slvglplus = softeq::lvglplus;

class SettingsPage : public slvglplus::Widget
{
public:
    static const std::string defaultAdjInTemperature;
    static const short colInTempBoxWidth;
    static const short titlePadding;
    static const short tempBoxBorderWidth;
    static const short tempBoxPadding;
    static const long inTempBoxColor;
    static const short tempTextWidth;
    static const short indentBetweenTempBoxAndButton;
    static const short indentBetweenTexts;
    static const short indentBetweenTempRows;
    static const short buttonSetWidth;
    static const short buttonSetHeight;
    static const short backButtonWidth;

    SettingsPage(const std::shared_ptr<NavigationBloc> &navigationBloc, const std::string &pageName,
                 const std::shared_ptr<slvglplus::TextEditingController> &insideTempCal);
};

#endif // LVGL_SETTINGS_PAGE_HH
