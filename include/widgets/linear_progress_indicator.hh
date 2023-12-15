#ifndef LVGL_PLUS_LINEAR_PROGRESS_INDICATOR_HH
#define LVGL_PLUS_LINEAR_PROGRESS_INDICATOR_HH

#include "properties/implementation/linear_progress_indicator_params.hh"
#include "widget.hh"
#include "widgets/properties/base/range_params.hh"

namespace softeq
{
namespace lvglplus
{

class LinearProgressIndicator : public Widget, public RangeOwner<ProgressBarRangeType>, public IndicatorOwner
{
public:
    LinearProgressIndicator();
    explicit LinearProgressIndicator(const LinearProgressIndicatorParamsBuilder &params);
    ~LinearProgressIndicator() override;
    void setRange(ProgressBarRangeType min, ProgressBarRangeType max) final;
    void setValue(ProgressBarRangeType value) final;
    ProgressBarRangeType min() const final;
    ProgressBarRangeType max() const final;
    ProgressBarRangeType value() const final;

private:
    void render() override;
};


} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_LINEAR_PROGRESS_INDICATOR_HH
