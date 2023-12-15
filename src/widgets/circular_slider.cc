#include <utility>

#include "lvgl_plus.hh"
#include "widgets/circular_slider.hh"

namespace softeq
{
namespace lvglplus
{

const short CircularSliderParams::defaultHeight = 120;
const short CircularSliderParams::defaultWidth = 120;

CircularSlider::~CircularSlider() = default;

CircularSlider::CircularSlider(const CircularSliderParamsBuilder &params)
    : Widget("CircularSlider", params.getParams(), lv_arc_create)
    , IndicatorOwner(lvglObject.getObject())
    , KnobOwner(lvglObject.getObject())
{
    auto circularSliderParams = params.getButtonParams();
    _onChangeCallback = circularSliderParams.onChangeCallback;
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::singleChild);
    lv_obj_add_event_cb(lvglObject.getObject(), &CircularSlider::onChange, LV_EVENT_VALUE_CHANGED, this);
    applyRangeParams(params.getRangeParams());
}

CircularSlider::CircularSlider()
    : CircularSlider(CircularSliderParamsBuilder())
{
}

void CircularSlider::onChange(_lv_event_t *event)
{
    if (((CircularSlider *)event->user_data)->_onChangeCallback)
    {
        ((CircularSlider *)event->user_data)->_onChangeCallback();
    }
}

void CircularSlider::render()
{
}

// todo pass the value as an argument
void CircularSlider::setOnChangeHandler(std::function<void(void)> handler)
{
    _onChangeCallback = std::move(handler);
}

CircularSliderRangeType CircularSlider::min() const
{
    return lv_arc_get_min_value(lvglObject.getObject());
}

CircularSliderRangeType CircularSlider::max() const
{
    return lv_arc_get_max_value(lvglObject.getObject());
}

CircularSliderRangeType CircularSlider::value() const
{
    return lv_arc_get_value(lvglObject.getObject());
}

void CircularSlider::setRange(CircularSliderRangeType min, CircularSliderRangeType max)
{
    UniqueLock lock(LvglPlus::lvglMutex());
    lv_arc_set_range(lvglObject.getObject(), min, max);
}

void CircularSlider::setValue(CircularSliderRangeType value)
{
    UniqueLock lock(LvglPlus::lvglMutex());
    lv_arc_set_value(lvglObject.getObject(), value);
}

} // namespace lvglplus
} // namespace softeq