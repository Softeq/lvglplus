#ifndef LVGL_PLUS_DROPDOWN_BUTTON_HH
#define LVGL_PLUS_DROPDOWN_BUTTON_HH

#include "properties/implementation/dropdown_button_params.hh"
#include "widget.hh"
#include <utility>

namespace softeq
{
namespace lvglplus
{

// todo find a way how to change the color of the list (maybe lvgl has a bug)
class DropDownButton : public Widget, public TextOwner, public IndicatorOwner
{
public:
    DropDownButton();
    explicit DropDownButton(const DropDownButtonParamsBuilder &params);
    ~DropDownButton() override;

    void setOptions(std::initializer_list<std::string> newOptions);
    int index() const;
    void setIndex(int index);
    std::string value();
    void setOnChangeHandler(std::function<void(void)> handler)
    {
        _onChangeCallback = std::move(handler);
    }

private:
    std::function<void(void)> _onChangeCallback;
    std::list<std::string> _options;
    int _index = 0;
    static void onChange(_lv_event_t *event);
    void render() override;
};

} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_DROPDOWN_BUTTON_HH
