#include "widgets/divider.hh"
#include "widgets/layouts/container.hh"

namespace softeq
{
namespace lvglplus
{

Divider::Divider(const DividerParamsBuilder &params)
    : Widget("LineSeparator", params.getParams())
{
    auto lineParams = params.getDividerParams();
    setBorderWidth(0);
    setPadding(Padding(0));
    setBackgroundOpacity(0);
    this->addChild(std::make_shared<Container>(
        ContainerParamsBuilder().backgroundColor(lineParams.lineColor).alignment(Alignment::center)));
    _lineSize = lineParams.lineThickness;
    _indent = lineParams.lineIndent;
}

Divider::Divider()
    : Divider(DividerParamsBuilder())
{
}

void Divider::render()
{
    // auto size divider line (make it vertical or horizontal) according to container size
    if (lv_obj_get_width(lvglObject.getObject()) < lv_obj_get_height(lvglObject.getObject()))
    {
        if (width() == LV_PCT(100) || width() == LV_SIZE_CONTENT)
        {
            setWidth(_indent);
        }
        if (height() == LV_PCT(100) || height() == LV_SIZE_CONTENT)
        {
            setHeight(LV_PCT(100));
        }
        (*_children->begin())->setWidth(_lineSize);
        (*_children->begin())->setHeight(LV_PCT(100));
    }
    else
    {
        if (width() == LV_PCT(100) || width() == LV_SIZE_CONTENT)
        {
            setWidth(LV_PCT(100));
        }
        if (height() == LV_PCT(100) || height() == LV_SIZE_CONTENT)
        {
            setHeight(_indent);
        }
        (*_children->begin())->setWidth(LV_PCT(100));
        (*_children->begin())->setHeight(_lineSize);
    }
}
} // namespace lvglplus
} // namespace softeq