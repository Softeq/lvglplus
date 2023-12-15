#include "widgets/layouts/container.hh"

namespace softeq
{
namespace lvglplus
{

const short ContainerParams::defaultHeight = LV_PCT(100);
const short ContainerParams::defaultWidth = LV_PCT(100);

Container::Container(const ContainerParamsBuilder &params)
    : Widget("Container", params.getParams(), lv_obj_create)
    , LayoutOwner(lvglObject.getObject())
{
    setMultiChild(params.getContainerParams().miltiChild);
    lv_obj_clear_flag(lvglObject.getObject(), LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
}

Container::Container()
    : Container(ContainerParamsBuilder())
{
}

void Container::render()
{
}

void Container::setMultiChild(bool enable)
{
    _children.reset();
    if (enable)
    {
        _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::multiChild);
    }
    else
    {
        _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::singleChild);
    }
}

Container::Container(lv_obj_t *lvObject)
    : Widget("Container", ContainerParamsBuilder().getParams())
    , LayoutOwner(lvObject)
{
    setMultiChild(true);
}

} // namespace lvglplus
} // namespace softeq