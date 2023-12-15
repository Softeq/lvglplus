#ifndef LVGL_METER_PARAMS_HH
#define LVGL_METER_PARAMS_HH

#include "widget.hh"
#include "widgets/properties/base/range_params.hh"
#include <utility>

using MeterRangeType = short;

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for the Meter widget
struct MeterParams
{
    static const short defaultWidth;
    static const short defaultHeight;
    unsigned short angle = 280;
    unsigned short angleOffset = 0;
    unsigned short ticksWidth = 3;
    unsigned short ticksLength = 15;
    unsigned short ticksMinorWidth = 2;
    unsigned short ticksMinorLength = 10;
    unsigned short ticksMinorQuantity = 21;
    unsigned short ticksEvery = 5;
    unsigned long tickColor = 0x606060;
    unsigned long tickMinorColor = 0xa0a0a0;
    unsigned short needleWidth = 3;
    unsigned long needleColor = 0;
    short needleExtender = -5;
    short labelGap = 10;
};

/*
 * The Builder class is used to set the MeterParams for the Meter widget that can be passed to the Meter constructor.
 * The Builder's setters return a reference to the MeterParamsBuilder object.
 */
template <typename T>
class MeterParamsBuilderBase : public WidgetParamsBuilder<T>, public RangeParamsBuilder<T, MeterRangeType>
{
protected:
    MeterParams _meterParams;

public:
    const MeterParams &getMeterParams() const
    {
        return _meterParams;
    }

    MeterParamsBuilderBase()
        : WidgetParamsBuilder<T>()
        , RangeParamsBuilder<T, MeterRangeType>()
    {
        this->width(MeterParams::defaultWidth);
        this->height(MeterParams::defaultHeight);
    }

    T &angle(unsigned short value)
    {
        _meterParams.angle = value;
        return this->retVal();
    }

    T &angleOffset(unsigned short value)
    {
        _meterParams.angleOffset = value;
        return this->retVal();
    }


    T &ticksWidth(unsigned short value)
    {
        _meterParams.ticksWidth = value;
        return this->retVal();
    }


    T &ticksLength(unsigned short value)
    {
        _meterParams.ticksLength = value;
        return this->retVal();
    }


    T &ticksMinorWidth(unsigned short value)
    {
        _meterParams.ticksMinorWidth = value;
        return this->retVal();
    }


    T &ticksMinorLength(unsigned short value)
    {
        _meterParams.ticksMinorLength = value;
        return this->retVal();
    }

    T &ticksMinorQuantity(unsigned short value)
    {
        _meterParams.ticksMinorQuantity = value;
        return this->retVal();
    }

    T &ticksEvery(unsigned short value)
    {
        _meterParams.ticksEvery = value;
        return this->retVal();
    }

    T &needleWidth(unsigned short value)
    {
        _meterParams.needleWidth = value;
        return this->retVal();
    }

    T &tickColor(unsigned long color)
    {
        _meterParams.tickColor = color;
        return this->retVal();
    }

    T &tickMinorColor(unsigned long color)
    {
        _meterParams.tickMinorColor = color;
        return this->retVal();
    }

    T &needleColor(unsigned long color)
    {
        _meterParams.needleColor = color;
        return this->retVal();
    }

    T &needleExtender(short value)
    {
        _meterParams.needleExtender = value;
        return this->retVal();
    }

    T &labelGap(short value)
    {
        _meterParams.labelGap = value;
        return this->retVal();
    }
};

class MeterParamsBuilder : public MeterParamsBuilderBase<MeterParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_METER_PARAMS_HH
