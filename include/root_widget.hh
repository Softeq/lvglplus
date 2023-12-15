#ifndef LVGL_PLUS_ROOT_WIDGET_HH
#define LVGL_PLUS_ROOT_WIDGET_HH

#include "widget.hh"

namespace softeq
{
namespace lvglplus
{

class RootWidget : public Widget
{
public:
    RootWidget(lv_disp_t *disp)
        : Widget("Root widget", lv_disp_get_scr_act(disp))
    {
        _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::multiChild);
    }

    ~RootWidget() = default;
};

} // namespace lvglplus
} // namespace softeq
#endif //LVGL_PLUS_ROOT_WIDGET_HH
