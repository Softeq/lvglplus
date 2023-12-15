#include "widgets/table.hh"
#include "lvgl_plus.hh"
#include <iostream>

namespace softeq
{
namespace lvglplus
{

const short TableParams::defaultWidth = LV_SIZE_CONTENT;
const short TableParams::defaultHeight = LV_SIZE_CONTENT;

Table::Table(const TableParamsBuilder &params)
    : Widget("Table", params.getParams(), lv_table_create)
    , TextOwner(this->lvglObject.getObject())
{
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::noChild);
    UniqueLock locker(LvglPlus::lvglMutex());
    auto object = lvglObject.getObject();
    auto tableParams = params.getTableParams();
    lv_table_set_row_cnt(object, tableParams.rowCount);
    lv_table_set_col_cnt(object, tableParams.columnCount);
    for (int i = 0; i < tableParams.rowCount; ++i)
    {
        lv_table_set_col_width(object, i, tableParams.columnWidth);
    }
}

Table::Table()
    : Table(TableParamsBuilder())
{
}

void Table::render()
{
}

void Table::setTitles(std::initializer_list<std::string> titles)
{
    auto lvObject = lvglObject.getObject();
    auto quantity = static_cast<short>(titles.size());
    if (quantity > lv_table_get_col_cnt(lvObject))
    {
        lv_table_set_col_cnt(lvObject, quantity);
        auto width = lv_table_get_col_width(lvObject, 0);
        for (int i = 0; i < quantity; ++i)
        {
            lv_table_set_col_width(lvObject, i, width);
        }
    }
    int i = 0;
    for (const auto &title : titles)
    {
        lv_table_set_cell_value(lvObject, 0, i, title.c_str());
        i++;
    }
}
void Table::addValues(std::initializer_list<std::string> values)
{
    auto lvObject = lvglObject.getObject();
    int row = lv_table_get_row_cnt(lvObject);
    int i = 0;
    for (const auto &value : values)
    {
        lv_table_set_cell_value(lvObject, row, i, value.c_str());
        i++;
    }
}
void Table::setValue(unsigned short row, unsigned short column, const std::string &value)
{
    lv_table_set_cell_value(lvglObject.getObject(), row, column, value.c_str());
}

} // namespace lvglplus
} // namespace softeq