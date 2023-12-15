#ifndef LVGL_PLUS_LINEAR_PROGRESS_INDICATOR_PARAMS_HH
#define LVGL_PLUS_LINEAR_PROGRESS_INDICATOR_PARAMS_HH

#include "widget.hh"
#include "widgets/properties/base/indicator_params.hh"
#include "widgets/properties/base/range_params.hh"

using ProgressBarRangeType = int;

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for the LinearProgressIndicator widget
struct LinearProgressIndicatorParams
{
    static const short defaultWidth;
    static const short defaultHeight;
};

/*
 * The Builder class is used to set the LinearProgressIndicatorParams for the LinearProgressIndicator widget that can be
 * passed to the LinearProgressIndicator constructor.
 * The Builder's setters return a reference to the LinearProgressIndicatorParamsBuilder object.
 */
template <typename T>
class LinearProgressIndicatorParamsBuilderBase : public WidgetParamsBuilder<T>,
                                                 public RangeParamsBuilder<T, ProgressBarRangeType>,
                                                 public IndicatorParamsBuilder<T>
{
protected:
    LinearProgressIndicatorParams _linearProgressIndicatorParams;

public:
    const LinearProgressIndicatorParams &getLinearProgressIndicatorParams() const
    {
        return _linearProgressIndicatorParams;
    }

    LinearProgressIndicatorParamsBuilderBase()
        : WidgetParamsBuilder<T>()
        , RangeParamsBuilder<T, ProgressBarRangeType>()
        , IndicatorParamsBuilder<T>()
    {
        this->width(LinearProgressIndicatorParams::defaultWidth);
        this->height(LinearProgressIndicatorParams::defaultHeight);
    }
};

class LinearProgressIndicatorParamsBuilder
    : public LinearProgressIndicatorParamsBuilderBase<LinearProgressIndicatorParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_LINEAR_PROGRESS_INDICATOR_PARAMS_HH
