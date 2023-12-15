#ifndef LVGL_PLUS_CIRCULAR_SLIDER_PARAMS_HH
#define LVGL_PLUS_CIRCULAR_SLIDER_PARAMS_HH

#include "widget.hh"
#include "widgets/properties/base/indicator_params.hh"
#include "widgets/properties/base/knob_params.hh"
#include "widgets/properties/base/range_params.hh"
#include <utility>

using CircularSliderRangeType = short;

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for the CircularSlider widget
struct CircularSliderParams
{
    static const short defaultWidth;
    static const short defaultHeight;
    std::function<void(void)> onChangeCallback = nullptr;
};

/*
 * The Builder class is used to set the CircularSliderParams for the CircularSlider widget that can be passed to the
 * CircularSlider constructor.
 * The Builder's setters return a reference to the CircularSliderParamsBuilder object.
 */
template <typename T>
class CircularSliderParamsBuilderBase : public WidgetParamsBuilder<T>,
                                        public RangeParamsBuilder<T, CircularSliderRangeType>,
                                        public IndicatorParamsBuilder<T>,
                                        public KnobParamsBuilder<T>
{
protected:
    CircularSliderParams _circularSliderParams;

public:
    const CircularSliderParams &getButtonParams() const
    {
        return _circularSliderParams;
    }

    CircularSliderParamsBuilderBase()
        : WidgetParamsBuilder<T>()
        , RangeParamsBuilder<T, CircularSliderRangeType>()
        , IndicatorParamsBuilder<T>()
        , KnobParamsBuilder<T>()
    {
        this->width(CircularSliderParams::defaultWidth);
        this->height(CircularSliderParams::defaultHeight);
    }

    T &onChangeHandler(std::function<void(void)> handler)
    {
        _circularSliderParams.onChangeCallback = std::move(handler);
        return this->retVal();
    }

    T &trackColor(long color)
    {
        this->addInstruction(
            [color](lv_obj_t *obj) { lv_obj_set_style_arc_color(obj, lv_color_hex(color), LV_PART_MAIN); });
        return this->retVal();
    }
};

class CircularSliderParamsBuilder : public CircularSliderParamsBuilderBase<CircularSliderParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_CIRCULAR_SLIDER_PARAMS_HH
