#ifndef LVGL_PLUS_TEXT_CONTROLLER_HH
#define LVGL_PLUS_TEXT_CONTROLLER_HH

#include "controller.hh"
#include "text_controllable.hh"
#include "widget.hh"
#include <string>

namespace softeq
{
namespace lvglplus
{

class TextController : public Controller
{
public:
    TextController()
        : TextController("")
    {
    }

    explicit TextController(std::string text)
        : _text(std::move(text))
    {
    }

    /// Set new text for controlled Widget with some text
    void setText(const std::string &newText)
    {
        _text = newText;
        if (auto widget = userWidget.lock())
        {
            if (auto userTextWidget = std::dynamic_pointer_cast<TextControllable>(widget))
            {
                userTextWidget->setText(newText);
            }
        }
    }
    /// Return current text from Widget with some text
    const std::string &getText()
    {
        return _text;
    }

protected:
    std::string _text;
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_PLUS_TEXT_CONTROLLER_HH
