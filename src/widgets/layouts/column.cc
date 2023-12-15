#include "widgets/layouts/column.hh"
#include "lvgl_plus.hh"
#include "widgets/layouts/container.hh"
#include <vector>

namespace softeq
{
namespace lvglplus
{
const short ColumnParams::defaultHeight = LV_SIZE_CONTENT;
const short ColumnParams::defaultWidth = LV_PCT(100);

std::shared_ptr<Widget> Column::addChild(const std::shared_ptr<Widget> &childWidget)
{
    if (childWidget->alignment() != Alignment::defaultAlignment)
    {
        auto cont =
            addChild(std::make_shared<Container>(ContainerParamsBuilder().width(LV_PCT(100)).heightEqContent()));
        cont->addChild(childWidget);
    }
    else
    {
        Widget::addChild(childWidget);
    }
    return childWidget;
}

Column::Column(const ColumnParamsBuilder &params)
    : Widget("Column", params.getParams(), lv_obj_create)
    , LayoutOwner(lvglObject.getObject())
{
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::multiChild);
    auto columnParams = params.getColumnParams();
    UniqueLock locker(LvglPlus::lvglMutex());
    auto object = lvglObject.getObject();
    //    lv_obj_set_layout(object, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(object, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(object, static_cast<lv_flex_align_t>(columnParams.justifyContent),
                          static_cast<lv_flex_align_t>(columnParams.alignItems),
                          static_cast<lv_flex_align_t>(columnParams.alignContent));
}

Column::Column()
    : Column(ColumnParamsBuilder())
{
}

void Column::render()
{
    // set height
    if (height() == LV_SIZE_CONTENT)
    {
        for (auto it = _children->begin(); *it != *(_children->end()); ++(*it))
        {
            if ((*it)->height() == LV_PCT(100))
            {
                (*it)->setHeight(LV_SIZE_CONTENT);
            }
        }
    }
    // set width
    if (width() == LV_SIZE_CONTENT)
    {
        short maxWidth = 0;
        std::vector<bool> resize = std::vector<bool>(_children->getChildrenQuantity());
        int i = 0;
        for (auto it = _children->begin(); *it != *(_children->end()); ++(*it))
        {
            if ((*it)->width() == LV_PCT(100))
            {
                (*it)->setWidth(LV_SIZE_CONTENT);
                lv_obj_update_layout((*it)->lvglObject.getObject());
            }
            maxWidth = std::max(maxWidth, lv_obj_get_width((*it)->lvglObject.getObject()));
            resize[i] = true;
            i++;
        }
        if (!resize.empty())
        {
            i = 0;
            for (auto it = _children->begin(); *it != *(_children->end()); ++(*it))
            {
                if (resize[i] && (lv_obj_get_width((*it)->lvglObject.getObject()) != maxWidth))
                {
                    (*it)->setWidth(maxWidth);
                    lv_obj_set_width((*it)->lvglObject.getObject(), maxWidth);
                    lv_obj_update_layout((*it)->lvglObject.getObject());
                }
                i++;
            }
        }
    }
}
} // namespace lvglplus
} // namespace softeq