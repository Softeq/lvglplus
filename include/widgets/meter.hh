#ifndef LVGL_PLUS_METER_HH
#define LVGL_PLUS_METER_HH

#include "properties/implementation/meter_params.hh"
#include "widget.hh"

namespace softeq
{
namespace lvglplus
{

class Meter : public Widget, public RangeOwner<MeterRangeType>
{
public:
    Meter();
    explicit Meter(const MeterParamsBuilder &params);
    ~Meter() override;
    void setRange(MeterRangeType min, MeterRangeType max) final;
    void setValue(MeterRangeType value) final;
    MeterRangeType min() const final;
    MeterRangeType max() const final;
    MeterRangeType value() const final;

private:
    unsigned short angle = 0;
    unsigned short angleOffset = 0;
    lv_meter_scale_t *scale;
    lv_meter_indicator_t *indic;
    void render() override;
};


} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_METER_HH
