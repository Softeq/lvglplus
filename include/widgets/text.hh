#ifndef LVGL_TEXT_HH
#define LVGL_TEXT_HH

#include "controllers/text_editing_controller.hh"
#include "properties/implementation/text_params.hh"
#include "text_controllable.hh"
#include "widget.hh"
#include "widget_containers/single_child_widget.hh"
#include <memory>

namespace softeq
{
namespace lvglplus
{

class Text : public Widget, public TextControllable, public TextOwner
{
public:
    Text();
    explicit Text(std::string text);
    Text(std::string text, const TextParamsBuilder &params);
    ~Text() override = default;
    void setController(const std::shared_ptr<Controller> &userController) override;

    std::shared_ptr<Controller> getController() override
    {
        return _controller.lock();
    }

    std::string getText() override;

    void setText(std::string) override;

private:
    std::weak_ptr<TextController> _controller;
    void render() override;
    std::string _text;
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_TEXT_HH
