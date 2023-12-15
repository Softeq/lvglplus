#ifndef LVGL_BUTTON_PARAMS_HH
#define LVGL_BUTTON_PARAMS_HH
#include "text_params.hh"
#include "widget.hh"

namespace softeq
{
namespace lvglplus
{

struct ButtonParams : public TextBasicParams
{
    static const short defaultWidth;
    static const short defaultHeight;
    std::function<void(void)> onClickCallback;
    bool hasText = false;
    bool hasImage = false;
    std::string imagePathLeft;
    std::string imagePathMid;
    std::string imagePathRight;
};

template <typename T>
class ButtonParamsBuilderBase : public WidgetParamsBuilder<T>, public TextBasicParamsBuilder<T>
{
protected:
    ButtonParams _buttonParams;

public:
    const ButtonParams &getButtonParams() const
    {
        return _buttonParams;
    }

    ButtonParamsBuilderBase()
        : WidgetParamsBuilder<T>()
        , TextBasicParamsBuilder<T>()
    {
        this->width(ButtonParams::defaultWidth);
        this->height(ButtonParams::defaultHeight);
    }

    T &onClickHandler(std::function<void(void)> handler)
    {
        _buttonParams.onClickCallback = std::move(handler);
        return this->retVal();
    }

    T &image(const std::string &pathLeft, const std::string &pathMid, const std::string &pathRight)
    {
        _buttonParams.imagePathLeft = pathLeft;
        _buttonParams.imagePathMid = pathMid;
        _buttonParams.imagePathRight = pathRight;
        _buttonParams.hasImage = true;
        return this->retVal();
    }
};

class ButtonParamsBuilder : public ButtonParamsBuilderBase<ButtonParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_BUTTON_PARAMS_HH
