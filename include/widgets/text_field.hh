#ifndef LVGL_TEXT_FIELD_HH
#define LVGL_TEXT_FIELD_HH

#include "controllers/text_editing_controller.hh"
#include "properties/implementation/text_field_params.hh"
#include "text_controllable.hh"
#include "widget.hh"
#include "widget_containers/single_child_widget.hh"

namespace softeq
{
namespace lvglplus
{

class TextField : public Widget, public TextControllable, public TextOwner
{
public:
    explicit TextField(std::string text);
    TextField(std::string text, const TextFieldParamsBuilder &params);
    void setController(const std::shared_ptr<Controller> &userController) override;
    std::shared_ptr<Controller> getController() override
    {
        return _controller.lock();
    }
    std::string getText() override;

private:
    std::string _text;
    std::weak_ptr<TextController> _controller;
    void setText(std::string) override;
    void render() override;
    static void onTextChanged(_lv_event_t *event);
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_TEXT_FIELD_HH
