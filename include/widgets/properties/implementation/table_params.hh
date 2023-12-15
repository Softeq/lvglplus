#ifndef LVGL_TABLE_PARAMS_HH
#define LVGL_TABLE_PARAMS_HH

#include "widget.hh"
#include "widget_containers/multi_child_widget.hh"
#include "widgets/properties/base/text_basic_params.hh"

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for the Table widget
struct TableParams : public TextBasicParams
{
    static const short defaultWidth;
    static const short defaultHeight;

    unsigned short rowCount = 1;
    unsigned short columnCount = 1;
    short columnWidth = 100;
};

/*
 * The Builder class is used to set the TableParams for the Table widget that can be passed to the Table constructor.
 * The Builder's setters return a reference to the TableParamsBuilder object.
 */
template <typename T>
class TableParamsBuilderBase : public WidgetParamsBuilder<T>, public TextBasicParamsBuilder<T>
{
protected:
    TableParams _tableParams;

public:
    const TableParams &getTableParams() const
    {
        return _tableParams;
    }

    TableParamsBuilderBase()
        : WidgetParamsBuilder<T>()
        , TextBasicParamsBuilder<T>()
    {
        this->width(TableParams::defaultWidth);
        this->height(TableParams::defaultHeight);
    }

    T &tableSize(short rows, short columns)
    {
        _tableParams.rowCount = rows;
        _tableParams.columnCount = columns;
        return this->retVal();
    }
};

class TableParamsBuilder : public TableParamsBuilderBase<TableParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_TABLE_PARAMS_HH
