#include "widgets/meter.hh"
#include "lvgl_plus.hh"

namespace softeq
{
namespace lvglplus
{

const short MeterParams::defaultHeight = 150;
const short MeterParams::defaultWidth = 150;

Meter::~Meter() = default;

Meter::Meter(const MeterParamsBuilder &params)
    : Widget("Meter", params.getParams(), lv_meter_create)
{
    auto meterParams = params.getMeterParams();
    angle = meterParams.angle;
    angleOffset = meterParams.angleOffset;
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::singleChild);
    auto lvObject = lvglObject.getObject();
    UniqueLock locker(LvglPlus::lvglMutex());
    scale = lv_meter_add_scale(lvObject);
    indic = lv_meter_add_needle_line(lvObject, scale, meterParams.needleWidth, lv_color_hex(meterParams.needleColor),
                                     meterParams.needleExtender);
    applyRangeParams(params.getRangeParams());
    lv_meter_set_scale_ticks(lvObject, scale, meterParams.ticksMinorQuantity, meterParams.ticksMinorWidth,
                             meterParams.ticksMinorLength, lv_color_hex(meterParams.tickMinorColor));
    lv_meter_set_scale_major_ticks(lvObject, scale, meterParams.ticksEvery, meterParams.ticksWidth,
                                   meterParams.ticksLength, lv_color_hex(meterParams.tickColor), meterParams.labelGap);
}

Meter::Meter()
    : Meter(MeterParamsBuilder())
{
}

void Meter::render()
{
}


MeterRangeType Meter::min() const
{
    return 0;
}

MeterRangeType Meter::max() const
{
    return 0;
}

MeterRangeType Meter::value() const
{
    return 0;
}

void Meter::setRange(MeterRangeType min, MeterRangeType max)
{
    unsigned short rotate = angleOffset + 90 + (360 - angle) / 2;
    UniqueLock locker(LvglPlus::lvglMutex());
    lv_meter_set_scale_range(lvglObject.getObject(), scale, min, max, angle, rotate);
}

void Meter::setValue(MeterRangeType value)
{
    UniqueLock locker(LvglPlus::lvglMutex());
    lv_meter_set_indicator_value(lvglObject.getObject(), indic, value);
}

} // namespace lvglplus
} // namespace softeq