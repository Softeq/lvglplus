#ifndef LVGL_PLUS_RANGE_PARAMS_HH
#define LVGL_PLUS_RANGE_PARAMS_HH

#include "lvgl.h"
#include "widgets/properties/base/base_params.hh"

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for widgets with a values range
template <typename paramType>
struct RangeParams
{
    paramType value = 0;
    paramType min = 0;
    paramType max = 100;
};

/*
 * The Builder class is used to set the RangeParams for the widget with a values range that can be passed as a part of
 * the base Builder to the widget constructor.
 * The Builder's setters return a reference to the templated (derived) Builder object.
 */
template <typename T, typename paramType>
class RangeParamsBuilder : virtual public BaseParamsBuilder<T>
{
private:
    RangeParams<paramType> _rangeParams;

public:
    const RangeParams<paramType> &getRangeParams() const
    {
        return _rangeParams;
    }

    RangeParamsBuilder() = default;

    T &range(paramType min, paramType max)
    {
        _rangeParams.min = min;
        _rangeParams.max = max;
        return this->retVal();
    }

    T &value(paramType value)
    {
        _rangeParams.value = value;
        return this->retVal();
    }
};

template <typename paramType>
class RangeOwner
{
public:
    virtual void setRange(paramType min, paramType max) = 0;
    virtual void setValue(paramType value) = 0;
    virtual paramType min() const = 0;
    virtual paramType max() const = 0;
    virtual paramType value() const = 0;

    void applyRangeParams(const RangeParams<paramType> &params)
    {
        setRange(params.min, params.max);
        setValue(params.value);
    }

protected:
    RangeOwner() = default;
};

} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_RANGE_PARAMS_HH
