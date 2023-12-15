#ifndef LVGL_ROW_HH
#define LVGL_ROW_HH

#include "widget.hh"
#include "widget_containers/multi_child_widget.hh"
#include "widgets/properties/base/layout_params.hh"
#include "widgets/properties/implementation/row_params.hh"

namespace softeq
{
namespace lvglplus
{

class Row : public Widget, public LayoutOwner
{
public:
    Row();
    explicit Row(const RowParamsBuilder &params);
    std::shared_ptr<Widget> addChild(const std::shared_ptr<Widget> &childWidget) override;

private:
    RowParams _rowParams;
    void render() override;
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_ROW_HH
