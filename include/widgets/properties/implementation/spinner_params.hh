#ifndef LVGL_SPINNER_PARAMS_HH
#define LVGL_SPINNER_PARAMS_HH

#include "widget.hh"
#include "widget_containers/multi_child_widget.hh"
#include "widgets/properties/base/indicator_params.hh"
#include "widgets/properties/base/text_basic_params.hh"

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for the Spinner widget
struct SpinnerParams
{
    static const short defaultWidth;
    static const short defaultHeight;
    static const std::string defaultText;

    unsigned int spinTime = 1000;
    unsigned int arcLength = 40;
};

/*
 * The Builder class is used to set the SpinnerParams for the Spinner widget that can be passed to the Spinner
 * constructor.
 * The Builder's setters return a reference to the SpinnerParamsBuilder object.
 */
template <typename T>
class SpinnerParamsBuilderBase : public WidgetParamsBuilder<T>, public IndicatorParamsBuilder<T>
{
protected:
    SpinnerParams _spinnerParams;

public:
    const SpinnerParams &getSpinnerParams() const
    {
        return _spinnerParams;
    }

    SpinnerParamsBuilderBase()
        : WidgetParamsBuilder<T>()
        , IndicatorParamsBuilder<T>()
    {
        this->width(SpinnerParams::defaultWidth);
        this->height(SpinnerParams::defaultHeight);
    }
};

class SpinnerParamsBuilder : public SpinnerParamsBuilderBase<SpinnerParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_SPINNER_PARAMS_HH
