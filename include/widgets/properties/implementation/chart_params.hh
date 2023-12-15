#ifndef LVGL_CHART_PARAMS_HH
#define LVGL_CHART_PARAMS_HH

#include "widget.hh"

namespace softeq
{
namespace lvglplus
{

struct ChartParams
{
    struct SeriesParams
    {
        long color = 0xff0000;
        SeriesParams(long color_)
        {
            color = color_;
        }
    };
    static const short defaultWidth;
    static const short defaultHeight;
    short verticalLines = 5;
    short horizintalLines = 5;
    short pointCount = 10;
    short xMin = 0;
    short xMax = 15;
    short yMin = 0;
    short yMax = 100;
    long lineColor = 0xb0b0b0;
    long textColor = 0;
    std::list<SeriesParams> series;
};

template <typename T>
class ChartParamsBuilderBase : public WidgetParamsBuilder<T>
{
protected:
    ChartParams _chartParams;

public:
    const ChartParams &getChartParams() const
    {
        return _chartParams;
    }

    ChartParamsBuilderBase()
        : WidgetParamsBuilder<T>()
    {
        this->width(ChartParams::defaultWidth);
        this->height(ChartParams::defaultHeight);
    }

    T &addSeries(long color)
    {
        _chartParams.series.emplace_back(color);
        return this->retVal();
    }

    T &rangeX(short min, short max)
    {
        _chartParams.xMin = min;
        _chartParams.xMax = max;
        return this->retVal();
    }

    T &rangeY(short min, short max)
    {
        _chartParams.yMin = min;
        _chartParams.yMax = max;
        return this->retVal();
    }

    T &pointCount(short value)
    {
        _chartParams.pointCount = value;
        return this->retVal();
    }
};

class ChartParamsBuilder : public ChartParamsBuilderBase<ChartParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_CHART_PARAMS_HH
