#ifndef LVGL_CONTAINER_HH
#define LVGL_CONTAINER_HH

#include "lvgl.h"
#include "widget.hh"
#include "widgets/properties/base/layout_params.hh"
#include "widgets/properties/implementation/container_params.hh"

namespace softeq
{
namespace lvglplus
{

class Container : public Widget, public LayoutOwner
{
public:
    Container();
    Container(lv_obj_t *lvObject);
    explicit Container(const ContainerParamsBuilder &params);
    ~Container() override = default;

    void setMultiChild(bool enable);

private:
    void render() override;
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_CONTAINER_HH
