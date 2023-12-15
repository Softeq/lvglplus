#ifndef LVGL_LAYOUT_PARAMS_HH
#define LVGL_LAYOUT_PARAMS_HH

#include "lvgl.h"

namespace softeq
{
namespace lvglplus
{

class LayoutOwner
{
private:
    lv_style_t style{};

protected:
    LayoutOwner(lv_obj_t *lvObject)
    {
        lv_style_init(&style);
        lv_style_set_bg_opa(&style, 0);
        lv_style_set_pad_all(&style, 0);
        lv_style_set_pad_gap(&style, 0);
        lv_style_set_border_width(&style, 0);
        lv_obj_add_style(lvObject, &style, 0);
    }
};

} // namespace lvglplus
} // namespace softeq

#endif //LVGL_LAYOUT_PARAMS_HH
