#include "widgets/list_wheel_scroll_view.hh"
#include <initializer_list>

namespace softeq
{
namespace lvglplus
{

const short ListWheelScrollViewParams::defaultHeight = 60;
const short ListWheelScrollViewParams::defaultWidth = 120;

ListWheelScrollView::~ListWheelScrollView() = default;

ListWheelScrollView::ListWheelScrollView(const ListWheelScrollViewParamsBuilder &params)
    : Widget("ListWheelScrollView", params.getParams(), lv_roller_create)
    , TextOwner(this->lvglObject.getObject())
    , IndicatorOwner(lvglObject.getObject())
{
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::singleChild);
    auto rollerParams = params.getListWheelScrollViewParams();
    _onChangeCallback = rollerParams.onChangeCallback;
    auto lvObject = lvglObject.getObject();
    lv_obj_add_event_cb(lvObject, &ListWheelScrollView::onChange, LV_EVENT_VALUE_CHANGED, this);
    setIndex(_index);
}

ListWheelScrollView::ListWheelScrollView()
    : ListWheelScrollView(ListWheelScrollViewParamsBuilder())
{
}

void ListWheelScrollView::onChange(_lv_event_t *event)
{
    ((ListWheelScrollView *)event->user_data)->_index = lv_roller_get_selected(event->target);
    if (((ListWheelScrollView *)event->user_data)->_onChangeCallback)
    {
        ((ListWheelScrollView *)event->user_data)->_onChangeCallback();
    }
}

void ListWheelScrollView::render()
{
}

void ListWheelScrollView::setOptions(std::initializer_list<std::string> newOptions)
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
    lv_roller_set_options(lvglObject.getObject(), optionsString.c_str(), LV_ROLLER_MODE_NORMAL);
}

int ListWheelScrollView::index() const
{
    return _index;
}

std::string ListWheelScrollView::value()
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

void ListWheelScrollView::setOnChangeHandler(std::function<void(void)> handler)
{
    _onChangeCallback = std::move(handler);
}

void ListWheelScrollView::setIndex(int index)
{
    _index = index;
    if (lvglObject.getObject())
    {
        lv_roller_set_selected(lvglObject.getObject(), index, LV_ANIM_ON);
    }
}

} // namespace lvglplus
} // namespace softeq