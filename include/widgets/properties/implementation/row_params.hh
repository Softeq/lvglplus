#ifndef LVGL_ROW_PARAMS_HH
#define LVGL_ROW_PARAMS_HH

#include "widget.hh"
#include "widget_containers/multi_child_widget.hh"

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for the LinearProgressIndicator widget
struct RowParams
{
    static const short defaultWidth;
    static const short defaultHeight;
    ChildrenAlignment justifyContent = ChildrenAlignment::start;
    ChildrenAlignment alignItems = ChildrenAlignment::start;
    ChildrenAlignment alignContent = ChildrenAlignment::start;
};

/*
 * The Builder class is used to set the RowParams for the Row widget that can be passed to the Row constructor.
 * The Builder's setters return a reference to the RowParamsBuilder object.
 */
template <typename T>
class RowParamsBuilderBase : public WidgetParamsBuilder<T>
{
protected:
    RowParams _rowParams;

public:
    const RowParams &getRowParams() const
    {
        return _rowParams;
    }

    RowParamsBuilderBase()
    {
        this->width(RowParams::defaultWidth);
        this->height(RowParams::defaultHeight);
    }

    T &justifyContent(ChildrenAlignment justifyContent)
    {
        _rowParams.justifyContent = justifyContent;
        return this->retVal();
    }

    T &alignItems(ChildrenAlignment alignItems)
    {
        _rowParams.alignItems = alignItems;
        return this->retVal();
    }

    T &alignContent(ChildrenAlignment alignContent)
    {
        _rowParams.alignContent = alignContent;
        return this->retVal();
    }
};

class RowParamsBuilder : public RowParamsBuilderBase<RowParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_ROW_PARAMS_HH
