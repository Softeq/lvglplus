#ifndef LVGL_TABLE_HH
#define LVGL_TABLE_HH

#include "properties/implementation/table_params.hh"
#include "widget.hh"

namespace softeq
{
namespace lvglplus
{

class Table : public Widget, public std::enable_shared_from_this<Table>, public TextOwner
{
public:
    Table();
    explicit Table(const TableParamsBuilder &params);
    ~Table() override = default;

    void setTitles(std::initializer_list<std::string> titles);
    void addValues(std::initializer_list<std::string> values);
    void setValue(unsigned short row, unsigned short column, const std::string &value);

private:
    void render() override;
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_TABLE_HH
