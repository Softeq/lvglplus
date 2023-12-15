#ifndef LVGL_DIVIDER_HH
#define LVGL_DIVIDER_HH

#include "lvgl.h"
#include "properties/implementation/divider_params.hh"
#include "widget.hh"
#include "widget_containers/single_child_widget.hh"

namespace softeq
{
namespace lvglplus
{

class Divider : public Widget
{
public:
    Divider();
    explicit Divider(const DividerParamsBuilder &params);
    ~Divider() override = default;

private:
    unsigned short _lineSize = 0;
    unsigned short _indent = 0;
    void render() override;
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_DIVIDER_HH
