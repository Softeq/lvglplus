#ifndef LVGL_LINE_SEPARATOR_PARAMS_HH
#define LVGL_LINE_SEPARATOR_PARAMS_HH

#include "lvgl.h"
#include "widget.hh"
#include "widget_containers/single_child_widget.hh"

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for the Divider widget
struct DividerParams
{
    unsigned short lineThickness = 1;
    unsigned short lineIndent = 15;
    long lineColor = 0x505050;
};

/*
 * The Builder class is used to set the DividerParams for the Divider widget that can be passed to the Divider
 * constructor.
 * The Builder's setters return a reference to the DividerParamsBuilder object.
 */
template <typename T>
class DividerParamsBuilderBase : public WidgetParamsBuilder<T>
{
protected:
    DividerParams _dividerParams;

public:
    const DividerParams &getDividerParams() const
    {
        return _dividerParams;
    }

    DividerParamsBuilderBase()
    {
        this->width(LV_PCT(100));
        this->height(LV_PCT(100));
    }

    T &lineThickness(unsigned short value)
    {
        _dividerParams.lineThickness = value;
        return this->retVal();
    }

    T &lineIndent(unsigned short value)
    {
        _dividerParams.lineIndent = value;
        return this->retVal();
    }

    T &lineColor(long color)
    {
        _dividerParams.lineColor = color;
        return this->retVal();
    }
};

class DividerParamsBuilder : public DividerParamsBuilderBase<DividerParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_LINE_SEPARATOR_PARAMS_HH
