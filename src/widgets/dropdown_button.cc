#include "widgets/dropdown_button.hh"

namespace softeq
{
namespace lvglplus
{

const short DropDownButtonParams::defaultHeight = LV_SIZE_CONTENT;
const short DropDownButtonParams::defaultWidth = 100;

DropDownButton::~DropDownButton() = default;

DropDownButton::DropDownButton(const DropDownButtonParamsBuilder &params)
    : Widget("DropDownButton", params.getParams(), lv_dropdown_create)
    , TextOwner(this->lvglObject.getObject())
    , IndicatorOwner(lvglObject.getObject())
{
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::singleChild);
    auto dropDownParams = params.getDropDownButtonParams();
    _onChangeCallback = dropDownParams.onChangeCallback;
    auto lvObject = lvglObject.getObject();
    lv_obj_add_event_cb(lvObject, &DropDownButton::onChange, LV_EVENT_VALUE_CHANGED, this);
    setIndex(_index);
}

DropDownButton::DropDownButton()
    : DropDownButton(DropDownButtonParamsBuilder())
{
}

void DropDownButton::onChange(_lv_event_t *event)
{
    ((DropDownButton *)event->user_data)->_index = lv_dropdown_get_selected(event->current_target);
    if (((DropDownButton *)event->user_data)->_onChangeCallback)
    {
        ((DropDownButton *)event->user_data)->_onChangeCallback();
    }
}

void DropDownButton::render()
{
}

void DropDownButton::setOptions(std::initializer_list<std::string> newOptions)
{
    for (const auto &str : newOptions)
    {
        _options.push_back(str);
    }
    std::string optionsString;
    long unsigned int i = 0;
    for (const auto &str : _options)
    {
        optionsString += str;
        i++;
        if (i < _options.size())
        {
            optionsString += "\n";
        }
    }
    lv_dropdown_set_options(lvglObject.getObject(), optionsString.c_str());
}

int DropDownButton::index() const
{
    return _index;
}

void DropDownButton::setIndex(int index)
{
    _index = index;
    if (lvglObject.getObject())
    {
        lv_dropdown_set_selected(lvglObject.getObject(), index);
    }
}

std::string DropDownButton::value()
{
    int i = 0;
    for (auto label : _options)
    {
        if (i == _index)
        {
            return label;
        }
        i++;
    }
    return "";
}

} // namespace lvglplus
} // namespace softeq