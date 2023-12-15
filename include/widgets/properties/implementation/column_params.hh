#ifndef LVGL_COLUMN_PARAMS_HH
#define LVGL_COLUMN_PARAMS_HH

#include "widget.hh"
#include "widget_containers/multi_child_widget.hh"

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for the Column widget
struct ColumnParams
{
    static const short defaultWidth;
    static const short defaultHeight;
    static const short defaultBackgroundOpacity;
    ChildrenAlignment justifyContent = ChildrenAlignment::start;
    ChildrenAlignment alignItems = ChildrenAlignment::start;
    ChildrenAlignment alignContent = ChildrenAlignment::start;
};

/*
 * The Builder class is used to set the ColumnParams for the Column widget that can be passed to the Column
 * constructor.
 * The Builder's setters return a reference to the ColumnParamsBuilder object.
 */
template <typename T>
class ColumnParamsBuilderBase : public WidgetParamsBuilder<T>
{
protected:
    ColumnParams _columnParams;

public:
    const ColumnParams &getColumnParams() const
    {
        return _columnParams;
    }

    ColumnParamsBuilderBase(const WidgetParams &widgetParams)
    {
        this->_widgetParams = widgetParams;
    }

    ColumnParamsBuilderBase()
    {
        this->width(ColumnParams::defaultWidth);
        this->height(ColumnParams::defaultHeight);
    }

    T &justifyContent(ChildrenAlignment justifyContent)
    {
        _columnParams.justifyContent = justifyContent;
        return this->retVal();
    }

    T &alignItems(ChildrenAlignment alignItems)
    {
        _columnParams.alignItems = alignItems;
        return this->retVal();
    }

    T &alignContent(ChildrenAlignment alignContent)
    {
        _columnParams.alignContent = alignContent;
        return this->retVal();
    }
};

class ColumnParamsBuilder : public ColumnParamsBuilderBase<ColumnParamsBuilder>
{
public:
    ColumnParamsBuilder(const WidgetParams &widgetParams)
        : ColumnParamsBuilderBase<ColumnParamsBuilder>(widgetParams)
    {
    }
    ColumnParamsBuilder()
        : ColumnParamsBuilderBase<ColumnParamsBuilder>()
    {
    }
};
} // namespace lvglplus
} // namespace softeq

#endif // LVGL_COLUMN_PARAMS_HH
