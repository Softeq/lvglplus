#ifndef LVGL_CHART_HH
#define LVGL_CHART_HH

#include "lvgl_plus.hh"
#include "properties/implementation/chart_params.hh"
#include <vector>

namespace softeq
{
namespace lvglplus
{

class Chart : public Widget, public std::enable_shared_from_this<Chart>
{
public:
    class ChartSeries
    {
    public:
        void addValue(short value)
        {
            UniqueLock locker(LvglPlus::lvglMutex());
            lv_chart_set_next_value(_lvObject, _ser, value);
        }
        ChartSeries(lv_obj_t *lvObject, lv_chart_series_t *ser)
        {
            _ser = ser;
            _lvObject = lvObject;
        }

    private:
        lv_chart_series_t *_ser;
        lv_obj_t *_lvObject;
    };
    Chart();
    explicit Chart(const ChartParamsBuilder &params);
    ~Chart() override = default;

    const std::vector<ChartSeries> &series() const
    {
        return _series;
    }

private:
    std::vector<ChartSeries> _series;
    void render() override;
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_CHART_HH
