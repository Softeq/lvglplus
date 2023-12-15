#ifndef LVGL_PLUS_SLIDER_PARAMS_HH
#define LVGL_PLUS_SLIDER_PARAMS_HH

#include <utility>

#include "widget.hh"
#include "widgets/properties/base/indicator_params.hh"
#include "widgets/properties/base/knob_params.hh"
#include "widgets/properties/base/range_params.hh"

using SliderRangeType = int;

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for the Slider widget
struct SliderParams
{
    static const short defaultWidth;
    static const short defaultHeight;
    std::function<void(void)> onChangeCallback;
};

/*
 * The Builder class is used to set the SliderParams for the Slider widget that can be passed to the Slider
 * constructor.
 * The Builder's setters return a reference to the SliderParamsBuilder object.
 */
template <typename T>
class SliderParamsBuilderBase : public WidgetParamsBuilder<T>,
                                public RangeParamsBuilder<T, SliderRangeType>,
                                public IndicatorParamsBuilder<T>,
                                public KnobParamsBuilder<T>
{
protected:
    SliderParams _sliderParams;

public:
    const SliderParams &getSliderParams() const
    {
        return _sliderParams;
    }

    SliderParamsBuilderBase()
        : WidgetParamsBuilder<T>()
        , RangeParamsBuilder<T, SliderRangeType>()
        , IndicatorParamsBuilder<T>()
        , KnobParamsBuilder<T>()
    {
        this->width(SliderParams::defaultWidth);
        this->height(SliderParams::defaultHeight);
    }

    T &onChangeHandler(std::function<void(void)> handler)
    {
        _sliderParams.onChangeCallback = std::move(handler);
        return this->retVal();
    }
};

class SliderParamsBuilder : public SliderParamsBuilderBase<SliderParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_SLIDER_PARAMS_HH
