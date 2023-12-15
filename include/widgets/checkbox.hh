#ifndef LVGL_PLUS_CHECKBOX_HH
#define LVGL_PLUS_CHECKBOX_HH

#include "properties/implementation/checkbox_params.hh"
#include "widget.hh"
#include <utility>

namespace softeq
{
namespace lvglplus
{

class Checkbox : public Widget, public TextOwner, public IndicatorOwner
{
public:
    Checkbox();
    explicit Checkbox(const CheckboxParamsBuilder &params);
    ~Checkbox() override;
    bool state() const;
    void setState(bool state);
    void setOnChangeHandler(std::function<void(void)> handler)
    {
        _onChangeCallback = std::move(handler);
    }

    void setIndicatorColor(long color) final
    {
        lv_obj_set_style_bg_color(lvglObject.getObject(), lv_color_hex(color), LV_PART_INDICATOR | LV_STATE_CHECKED);
    }
    long indicatorColor() final
    {
        return static_cast<long>(
            lv_obj_get_style_bg_color(lvglObject.getObject(), LV_PART_INDICATOR | LV_STATE_CHECKED).full & 0xffffff);
    }

    long inactiveColor()
    {
        return static_cast<long>(lv_obj_get_style_bg_color(lvglObject.getObject(), LV_PART_INDICATOR).full & 0xffffff);
    }
    void setInactiveColor(long color)
    {
        lv_obj_set_style_bg_color(lvglObject.getObject(), lv_color_hex(color), LV_PART_INDICATOR);
    }

private:
    std::function<void(void)> _onChangeCallback;
    bool _state = false;
    static void onChange(_lv_event_t *c);
    void render() override;
};

} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_CHECKBOX_HH
