#include "widgets/layouts/row.hh"
#include "lvgl_plus.hh"
#include "widgets/layouts/container.hh"
#include <vector>

namespace softeq
{
namespace lvglplus
{
const short RowParams::defaultHeight = LV_SIZE_CONTENT;
const short RowParams::defaultWidth = LV_SIZE_CONTENT;

std::shared_ptr<Widget> Row::addChild(const std::shared_ptr<Widget> &childWidget)
{
    if (childWidget->alignment() != Alignment::defaultAlignment)
    {
        auto cont =
            addChild(std::make_shared<Container>(ContainerParamsBuilder().height(LV_PCT(100)).widthEqContent()));
        cont->addChild(childWidget);
    }
    else
    {
        Widget::addChild(childWidget);
    }
    return childWidget;
}

Row::Row(const RowParamsBuilder &params)
    : Widget("Row", params.getParams(), lv_obj_create)
    , LayoutOwner(lvglObject.getObject())
{
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::multiChild);
    auto rowParams = params.getRowParams();
    auto object = lvglObject.getObject();
    UniqueLock locker(LvglPlus::lvglMutex());
    lv_obj_set_layout(object, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(object, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(object, static_cast<lv_flex_align_t>(rowParams.justifyContent),
                          static_cast<lv_flex_align_t>(rowParams.alignItems),
                          static_cast<lv_flex_align_t>(rowParams.alignContent));
}

Row::Row()
    : Row(RowParamsBuilder())
{
}

void Row::render()
{
    // set width
    if (width() == LV_SIZE_CONTENT)
    {
        for (auto it = _children->begin(); *it != *(_children->end()); ++(*it))
        {
            if ((*it)->width() == LV_PCT(100))
            {
                (*it)->setWidth(LV_SIZE_CONTENT);
            }
        }
    }
    // set height
    if (height() == LV_SIZE_CONTENT)
    {
        short maxHeight = 0;
        std::vector<bool> resize = std::vector<bool>(_children->getChildrenQuantity());
        int i = 0;
        for (auto it = _children->begin(); *it != *(_children->end()); ++(*it))
        {
            if ((*it)->height() == LV_PCT(100))
            {
                (*it)->setHeight(LV_SIZE_CONTENT);
                lv_obj_update_layout((*it)->lvglObject.getObject());
                resize[i] = true;
            }
            maxHeight = std::max(maxHeight, lv_obj_get_height((*it)->lvglObject.getObject()));
            i++;
        }
        if (!resize.empty())
        {
            i = 0;
            for (auto it = _children->begin(); *it != *(_children->end()); ++(*it))
            {
                if (resize[i] && (lv_obj_get_height((*it)->lvglObject.getObject()) != maxHeight))
                {
                    (*it)->setHeight(maxHeight);
                    lv_obj_set_height((*it)->lvglObject.getObject(), maxHeight);
                    lv_obj_update_layout((*it)->lvglObject.getObject());
                }
                i++;
            }
        }
    }
}
} // namespace lvglplus
} // namespace softeq