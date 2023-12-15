#ifndef LVGL_BUTTON_HH
#define LVGL_BUTTON_HH

#include "properties/implementation/button_params.hh"
#include "text.hh"
#include "widget.hh"

namespace softeq
{
namespace lvglplus
{

class Button : public Widget
{
public:
    Button();
    explicit Button(std::string text);
    Button(std::string text, const ButtonParamsBuilder &params);
    explicit Button(const ButtonParamsBuilder &params);
    ~Button() override;

    void setOnClickHandler(std::function<void(void)> handler)
    {
        onClickCallback = std::move(handler);
    }

    std::shared_ptr<Text> getTextWidget()
    {
        return _textWidget.lock();
    }

private:
    std::string pathLeft;
    std::string pathMid;
    std::string pathRight;
    std::weak_ptr<Text> _textWidget;
    static void onClick(_lv_event_t *c);
    std::function<void(void)> onClickCallback;
    void render() override;
    std::string _text;
    bool hasText = false;
};
} // namespace lvglplus
} // namespace softeq

#endif // LVGL_BUTTON_HH
