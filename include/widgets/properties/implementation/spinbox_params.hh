#ifndef LVGL_SPINBOX_PARAMS_HH
#define LVGL_SPINBOX_PARAMS_HH

#include "widget.hh"
#include "widget_containers/multi_child_widget.hh"
#include "widgets/properties/base/range_params.hh"
#include "widgets/properties/base/text_basic_params.hh"

using SpinboxRangeType = float;

namespace softeq
{
namespace lvglplus
{

struct SpinboxParams : public TextBasicParams
{
    static const short defaultWidth;
    static const short defaultHeight;

    std::function<void(void)> onChangeCallback = nullptr;
    unsigned char digits = 3;
    unsigned char separatorPosition = 0;
    SpinboxRangeType step = 1;
};

template <typename T>
class SpinboxParamsBuilderBase : public WidgetParamsBuilder<T>,
                                 public TextBasicParamsBuilder<T>,
                                 public RangeParamsBuilder<T, SpinboxRangeType>
{
protected:
    SpinboxParams _spinboxParams;

public:
    const SpinboxParams &getSpinboxParams() const
    {
        return _spinboxParams;
    }

    SpinboxParamsBuilderBase()
        : WidgetParamsBuilder<T>()
        , TextBasicParamsBuilder<T>()
        , RangeParamsBuilder<T, SpinboxRangeType>()
    {
        this->width(SpinboxParams::defaultWidth);
        this->height(SpinboxParams::defaultHeight);
    }

    T &onChangeHandler(std::function<void(void)> handler)
    {
        _spinboxParams.onChangeCallback = std::move(handler);
        return this->retVal();
    }

    T &digitFormat(unsigned char digits, unsigned char separatorPosition)
    {
        _spinboxParams.digits = digits;
        _spinboxParams.separatorPosition = separatorPosition;
        return this->retVal();
    }

    T &step(SpinboxRangeType value)
    {
        _spinboxParams.step = value;
        return this->retVal();
    }
};

class SpinboxParamsBuilder : public SpinboxParamsBuilderBase<SpinboxParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_SPINBOX_PARAMS_HH
