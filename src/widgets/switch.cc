#include "widgets/switch.hh"

namespace softeq
{
namespace lvglplus
{

const short SwitchParams::defaultHeight = 25;
const short SwitchParams::defaultWidth = 45;

Switch::~Switch() = default;

Switch::Switch(const SwitchParamsBuilder &params)
    : Widget("Switch", params.getParams(), lv_switch_create)
    , IndicatorOwner(lvglObject.getObject())
{
    const auto &switchParams = params.getButtonParams();
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::singleChild);
    auto lvObject = lvglObject.getObject();
    lv_obj_add_event_cb(lvObject, &Switch::onChange, LV_EVENT_CLICKED, this);
    setState(_state);
}

Switch::Switch()
    : Switch(SwitchParamsBuilder())
{
}

void Switch::onChange(_lv_event_t *event)
{
    ((Switch *)event->user_data)->_state = (lv_obj_get_state(event->current_target) & LV_STATE_CHECKED);
    if (((Switch *)event->user_data)->_onChangeCallback)
    {
        ((Switch *)event->user_data)->_onChangeCallback();
    }
}

void Switch::render()
{
}

bool Switch::state() const
{
    return _state;
}

void Switch::setState(bool state)
{
    _state = state;
    if (lvglObject.getObject())
    {
        if (state)
        {
            lv_obj_add_state(lvglObject.getObject(), LV_STATE_CHECKED);
        }
        else
        {
            lv_obj_clear_state(lvglObject.getObject(), LV_STATE_CHECKED);
        }
        lv_obj_invalidate(lvglObject.getObject());
    }
}

} // namespace lvglplus
} // namespace softeq