#ifndef LVGL_MAIN_PAGE_HH
#define LVGL_MAIN_PAGE_HH

#include "controllers/text_editing_controller.hh"
#include "navigation_bloc/navigation_bloc.hh"
#include "widget.hh"

namespace slvglplus = softeq::lvglplus;

class MainPage : public slvglplus::Widget
{
public:
    static const short colInTempBoxWidth;
    static const short settingsButtonWidth;
    static const short titlePadding;
    static const short tempBoxBorderWidth;
    static const short tempBoxPadding;
    static const long inTempBoxColor;
    static const long outTempBoxColor;
    static const short tempTextWidth;
    static const short indentBetweenTempBoxes;
    static const short indentBetweenTempBoxAndButton;

    MainPage(const std::shared_ptr<NavigationBloc> &navigationBloc, const std::string &pageName,
             const std::shared_ptr<slvglplus::TextEditingController> &insideTemp,
             const std::shared_ptr<slvglplus::TextEditingController> &outsideTemp,
             const std::shared_ptr<slvglplus::TextEditingController> &insideTempCal);
};

#endif // LVGL_MAIN_PAGE_HH
