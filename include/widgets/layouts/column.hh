#ifndef LVGL_COLUMN_HH
#define LVGL_COLUMN_HH

#include "widget.hh"
#include "widgets/properties/base/layout_params.hh"
#include "widgets/properties/implementation/column_params.hh"

namespace softeq
{
namespace lvglplus
{

class Column : public Widget, public LayoutOwner
{
public:
    Column();
    explicit Column(const ColumnParamsBuilder &params);
    std::shared_ptr<Widget> addChild(const std::shared_ptr<Widget> &childWidget) override;

private:
    void render() override;
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_COLUMN_HH
