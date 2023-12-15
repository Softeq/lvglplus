#ifndef LVGL_LED_PARAMS_HH
#define LVGL_LED_PARAMS_HH

#include "widget.hh"

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for the LED widget
struct LedParams
{
    static const short defaultWidth;
    static const short defaultHeight;
    unsigned char brightness = 0;
    bool state = false;
    long color = 0xff0000;
};

/*
 * The Builder class is used to set the LedParams for the LED widget that can be passed to the LED constructor.
 * The Builder's setters return a reference to the LedParamsBuilder object.
 */
template <typename T>
class LedParamsBuilderBase : public WidgetParamsBuilder<T>
{
protected:
    LedParams _ledParams;

public:
    const LedParams &getLedParams() const
    {
        return _ledParams;
    }

    LedParamsBuilderBase()
        : WidgetParamsBuilder<T>()
    {
        this->width(LedParams::defaultWidth);
        this->height(LedParams::defaultHeight);
    }

    T &state(bool state_)
    {
        _ledParams.state = state_;
        return this->retVal();
    }

    T &brightness(unsigned char brightness_)
    {
        _ledParams.state = brightness_;
        return this->retVal();
    }

    T &color(long color_)
    {
        _ledParams.color = color_;
        return this->retVal();
    }
};

class LedParamsBuilder : public LedParamsBuilderBase<LedParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_LED_PARAMS_HH
