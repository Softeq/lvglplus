#ifndef LVGL_TEXT_EDITING_CONTROLLER_HH
#define LVGL_TEXT_EDITING_CONTROLLER_HH

#include "text_controller.hh"
#include "widget.hh"
#include <string>

namespace softeq
{
namespace lvglplus
{

class TextEditingController : public TextController
{
public:
    TextEditingController(std::string text, std::function<void(std::string)> updateText)
        : TextController(std::move(text))
        , usrOnTextChangeCbk(std::move(updateText))
    {
    }

    explicit TextEditingController(std::string text)
        : TextController(std::move(text))
    {
    }

    explicit TextEditingController()
        : TextController()
    {
    }

    void setOnChangeHandler(std::function<void(const std::string &newText)> callback)
    {
        usrOnTextChangeCbk = std::move(callback);
    }

    /// Set new text for controlled Widget with some text
    void setTextWithNotify(const std::string &newText)
    {
        setText(newText);
        onTextChange(newText);
    }

    void onTextChange(const std::string &newText)
    {
        _text = newText;
        if (usrOnTextChangeCbk)
        {
            usrOnTextChangeCbk(newText);
        }
    }

private:
    std::function<void(std::string)> usrOnTextChangeCbk;
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_TEXT_EDITING_CONTROLLER_HH
