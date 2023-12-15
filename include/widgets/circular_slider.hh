#ifndef LVGL_PLUS_CIRCULAR_SLIDER_HH
#define LVGL_PLUS_CIRCULAR_SLIDER_HH

#include "properties/implementation/circular_slider_params.hh"
#include "widget.hh"
#include <utility>

namespace softeq
{
namespace lvglplus
{

class CircularSlider : public Widget,
                       public RangeOwner<CircularSliderRangeType>,
                       public IndicatorOwner,
                       public KnobOwner
{
public:
    CircularSlider();
    explicit CircularSlider(const CircularSliderParamsBuilder &params);
    ~CircularSlider() override;
    void setOnChangeHandler(std::function<void(void)> handler);

    void setRange(CircularSliderRangeType min, CircularSliderRangeType max) final;
    void setValue(CircularSliderRangeType value) final;
    CircularSliderRangeType min() const final;
    CircularSliderRangeType max() const final;
    CircularSliderRangeType value() const final;

    void setIndicatorColor(long color) final
    {
        UniqueLock lock(LvglPlus::lvglMutex());
        lv_obj_set_style_arc_color(lvglObject.getObject(), lv_color_hex(color), LV_PART_INDICATOR);
    }
    long indicatorColor() final
    {
        return static_cast<long>(lv_obj_get_style_arc_color(lvglObject.getObject(), LV_PART_INDICATOR).full & 0xffffff);
    }

    void setKnobColor(long color) final
    {
        UniqueLock lock(LvglPlus::lvglMutex());
        lv_obj_set_style_bg_color(lvglObject.getObject(), lv_color_hex(color), LV_PART_KNOB);
    }
    long knobColor() final
    {
        return static_cast<long>(lv_obj_get_style_bg_color(lvglObject.getObject(), LV_PART_KNOB).full & 0xffffff);
    }

    void setTrackColor(long color)
    {
        UniqueLock lock(LvglPlus::lvglMutex());
        lv_obj_set_style_arc_color(lvglObject.getObject(), lv_color_hex(color), LV_PART_MAIN);
    }
    long trackColor()
    {
        return static_cast<long>(lv_obj_get_style_arc_color(lvglObject.getObject(), LV_PART_MAIN).full & 0xffffff);
    }

private:
    std::function<void(void)> _onChangeCallback;
    static void onChange(_lv_event_t *c);
    void render() override;
};


} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_CIRCULAR_SLIDER_HH
