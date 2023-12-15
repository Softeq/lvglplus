#include "widgets/chart.hh"
#include "lvgl_plus.hh"

namespace softeq
{
namespace lvglplus
{

const short ChartParams::defaultHeight = 200;
const short ChartParams::defaultWidth = 200;

Chart::Chart(const ChartParamsBuilder &params)
    : Widget("Chart", params.getParams(), lv_chart_create)
{
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::noChild);
    auto chartParams = params.getChartParams();
    auto lvObject = lvglObject.getObject();
    UniqueLock locker(LvglPlus::lvglMutex());
    lv_chart_set_type(lvObject, LV_CHART_TYPE_LINE);
    lv_chart_set_update_mode(lvObject, LV_CHART_UPDATE_MODE_SHIFT);

    lv_chart_set_axis_tick(lvObject, LV_CHART_AXIS_PRIMARY_X, 10, 5, 5, 2, true, 50);
    lv_chart_set_axis_tick(lvObject, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 2, true, 50);

    for (auto series : chartParams.series)
    {
        _series.emplace_back(lvObject,
                             lv_chart_add_series(lvObject, lv_color_hex(series.color), LV_CHART_AXIS_PRIMARY_Y));
    }

    lv_chart_set_range(lvObject, LV_CHART_AXIS_PRIMARY_X, chartParams.xMin, chartParams.xMax);
    lv_chart_set_range(lvObject, LV_CHART_AXIS_PRIMARY_Y, chartParams.yMin, chartParams.yMax);

    lv_chart_set_div_line_count(lvObject, chartParams.verticalLines, chartParams.horizintalLines);
    lv_chart_set_point_count(lvObject, chartParams.pointCount);
    lv_obj_set_style_line_color(lvObject, lv_color_hex(chartParams.lineColor), LV_PART_MAIN);
    lv_obj_set_style_text_color(lvObject, lv_color_hex(chartParams.textColor), LV_PART_TICKS);
}

Chart::Chart()
    : Chart(ChartParamsBuilder())
{
}

void Chart::render()
{
}

} // namespace lvglplus
} // namespace softeq