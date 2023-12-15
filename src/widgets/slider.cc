#include "widgets/slider.hh"

namespace softeq
{
namespace lvglplus
{

const short SliderParams::defaultHeight = 15;
const short SliderParams::defaultWidth = 200;

Slider::~Slider() = default;

Slider::Slider(const SliderParamsBuilder &params)
    : Widget("Slider", params.getParams(), lv_slider_create)
    , IndicatorOwner(lvglObject.getObject())
    , KnobOwner(lvglObject.getObject())
{
    auto sliderParams = params.getSliderParams();
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::singleChild);
    _onChangeCallback = sliderParams.onChangeCallback;
    auto lvObject = lvglObject.getObject();
    applyRangeParams(params.getRangeParams());
    lv_obj_add_event_cb(lvObject, &Slider::onChange, LV_EVENT_VALUE_CHANGED, this);
}

Slider::Slider()
    : Slider(SliderParamsBuilder())
{
}

void Slider::onChange(_lv_event_t *event)
{
    if (((Slider *)event->user_data)->_onChangeCallback)
    {
        ((Slider *)event->user_data)->_onChangeCallback();
    }
}

void Slider::render()
{
}

SliderRangeType Slider::min() const
{
    return lv_slider_get_min_value(lvglObject.getObject());
}

SliderRangeType Slider::max() const
{
    return lv_slider_get_max_value(lvglObject.getObject());
}

SliderRangeType Slider::value() const
{
    return lv_slider_get_value(lvglObject.getObject());
}

void Slider::setRange(SliderRangeType min, SliderRangeType max)
{
    lv_slider_set_range(lvglObject.getObject(), min, max);
}

void Slider::setValue(SliderRangeType value)
{
    lv_slider_set_value(lvglObject.getObject(), value, LV_ANIM_OFF);
}

} // namespace lvglplus
} // namespace softeq