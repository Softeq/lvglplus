#ifndef LVGL_PLUS_SWITCH_PARAMS_HH
#define LVGL_PLUS_SWITCH_PARAMS_HH

#include "widget.hh"
#include "widgets/properties/base/indicator_params.hh"

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for the LinearProgressIndicator widget
struct SwitchParams
{
    static const short defaultWidth;
    static const short defaultHeight;
    std::function<void(void)> onChangeCallback;
};

/*
 * The Builder class is used to set the SwitchParams for the Switch widget that can be passed to the Switch
 * constructor.
 * The Builder's setters return a reference to the SwitchParamsBuilder object.
 */
template <typename T>
class SwitchParamsBuilderBase : public WidgetParamsBuilder<T>, public IndicatorParamsBuilder<T>
{
protected:
    SwitchParams _switchParams;

public:
    const SwitchParams &getButtonParams() const
    {
        return _switchParams;
    }

    SwitchParamsBuilderBase()
        : WidgetParamsBuilder<T>()
        , IndicatorParamsBuilder<T>()
    {
        this->width(SwitchParams::defaultWidth);
        this->height(SwitchParams::defaultHeight);
    }

    T &onChangeHandler(std::function<void(void)> handler)
    {
        _switchParams.onChangeCallback = std::move(handler);
        return this->retVal();
    }
};

class SwitchParamsBuilder : public SwitchParamsBuilderBase<SwitchParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq


#endif //LVGL_PLUS_SWITCH_PARAMS_HH
