#include "widgets/checkbox.hh"
#include <string>

namespace softeq
{
namespace lvglplus
{

const short CheckboxParams::defaultHeight = LV_SIZE_CONTENT;
const short CheckboxParams::defaultWidth = LV_SIZE_CONTENT;

Checkbox::Checkbox(const CheckboxParamsBuilder &params)
    : Widget("Checkbox", params.getParams(), lv_checkbox_create)
    , TextOwner(lvglObject.getObject())
    , IndicatorOwner(lvglObject.getObject())
{
    auto checkboxParams = params.getCheckboxParams();
    _onChangeCallback = checkboxParams.onChangeCallback;
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::singleChild);
    auto lvObject = lvglObject.getObject();
    lv_checkbox_set_text(lvObject, checkboxParams.text.c_str());
    setState(_state);
    lv_obj_add_event_cb(lvObject, &Checkbox::onChange, LV_EVENT_VALUE_CHANGED, this);
}

Checkbox::Checkbox()
    : Checkbox(CheckboxParamsBuilder())
{
}

void Checkbox::onChange(_lv_event_t *event)
{
    ((Checkbox *)event->user_data)->_state = (lv_obj_get_state(event->current_target) & LV_STATE_CHECKED) != 0;
    if (((Checkbox *)event->user_data)->_onChangeCallback)
    {
        ((Checkbox *)event->user_data)->_onChangeCallback();
    }
}

void Checkbox::render()
{
}

bool Checkbox::state() const
{
    return _state;
}

void Checkbox::setState(bool state)
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

Checkbox::~Checkbox() = default;

} // namespace lvglplus
} // namespace softeq