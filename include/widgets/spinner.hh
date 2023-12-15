#ifndef LVGL_PLUS_SPINNER_HH
#define LVGL_PLUS_SPINNER_HH

#include "lvgl_plus.hh"
#include "properties/implementation/spinner_params.hh"
#include "widget.hh"
#include <utility>

namespace softeq
{
namespace lvglplus
{

class Spinner : public Widget, public IndicatorOwner
{
public:
    Spinner();
    explicit Spinner(const SpinnerParamsBuilder &params);
    ~Spinner() override;

    void setIndicatorColor(long color) final
    {
        lv_obj_set_style_arc_color(lvglObject.getObject(), lv_color_hex(color), LV_PART_INDICATOR);
    }
    long indicatorColor() final
    {
        return static_cast<long>(lv_obj_get_style_arc_color(lvglObject.getObject(), LV_PART_INDICATOR).full & 0xffffff);
    }

private:
    void render() override;
};

} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_SPINNER_HH
