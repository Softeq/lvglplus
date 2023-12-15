#ifndef LVGL_PLUS_SLIDER_HH
#define LVGL_PLUS_SLIDER_HH

#include <utility>

#include "properties/implementation/slider_params.hh"
#include "widget.hh"

namespace softeq
{
namespace lvglplus
{

class Slider : public Widget, public RangeOwner<SliderRangeType>, public IndicatorOwner, public KnobOwner
{
public:
    Slider();
    explicit Slider(const SliderParamsBuilder &params);
    ~Slider() override;
    void setRange(SliderRangeType min, SliderRangeType max) final;
    void setValue(SliderRangeType value) final;
    SliderRangeType min() const final;
    SliderRangeType max() const final;
    SliderRangeType value() const final;

    void setOnChangeHandler(std::function<void(void)> handler)
    {
        _onChangeCallback = std::move(handler);
    }

private:
    std::function<void(void)> _onChangeCallback;
    static void onChange(_lv_event_t *event);
    void render() override;
};


} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_SLIDER_HH
