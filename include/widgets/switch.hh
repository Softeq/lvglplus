#ifndef LVGL_PLUS_SWITCH_HH
#define LVGL_PLUS_SWITCH_HH

#include "properties/implementation/switch_params.hh"
#include "widget.hh"
#include "widgets/properties/base/indicator_params.hh"

namespace softeq
{
namespace lvglplus
{

class Switch : public Widget, public IndicatorOwner
{
public:
    Switch();
    explicit Switch(const SwitchParamsBuilder &params);
    ~Switch() override;
    bool state() const;
    void setState(bool state);

    void setOnChangeHandler(std::function<void(void)> handler)
    {
        _onChangeCallback = std::move(handler);
    }

    void setIndicatorColor(long color) final
    {
        lv_obj_set_style_bg_color(_lvObject, lv_color_hex(color), LV_PART_INDICATOR | LV_STATE_CHECKED);
    }

private:
    std::function<void(void)> _onChangeCallback;
    bool _state = false;
    static void onChange(_lv_event_t *event);
    void render() override;
};


} // namespace lvglplus
} // namespace softeq


#endif //LVGL_PLUS_SWITCH_HH
