#ifndef LVGL_PLUS_LIST_WHEEL_SCROLL_VIEW_HH
#define LVGL_PLUS_LIST_WHEEL_SCROLL_VIEW_HH

#include "properties/implementation/list_wheel_scroll_view_params.hh"
#include "widget.hh"
#include <utility>

namespace softeq
{
namespace lvglplus
{

class ListWheelScrollView : public Widget, public TextOwner, public IndicatorOwner
{
public:
    ListWheelScrollView();
    explicit ListWheelScrollView(const ListWheelScrollViewParamsBuilder &params);
    ~ListWheelScrollView() override;
    void setOptions(std::initializer_list<std::string> newOptions);
    int index() const;
    std::string value();
    void setIndex(int index);
    void setOnChangeHandler(std::function<void(void)> handler);

    void setIndicatorColor(long color) final
    {
        lv_obj_set_style_bg_color(lvglObject.getObject(), lv_color_hex(color), LV_PART_SELECTED);
    }
    long indicatorColor() final
    {
        return static_cast<long>(lv_obj_get_style_bg_color(lvglObject.getObject(), LV_PART_SELECTED).full & 0xffffff);
    }

private:
    std::list<std::string> _options;
    std::function<void(void)> _onChangeCallback;
    int _index = 0;
    static void onChange(_lv_event_t *event);
    void render() override;
};


} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_LIST_WHEEL_SCROLL_VIEW_HH
