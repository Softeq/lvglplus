#include <utility>

#include "lvgl_plus.hh"
#include "widgets/divider.hh"
#include "widgets/list.hh"

namespace softeq
{
namespace lvglplus
{

const short ListParams::defaultHeight = LV_SIZE_CONTENT;
const short ListParams::defaultWidth = 200;

List::~List() = default;

List::List(const ListParamsBuilder &params)
    : Widget("List", params.getParams())
{
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::multiChild);
    UniqueLock locker(LvglPlus::lvglMutex());
    auto object = lvglObject.getObject();
    lv_obj_set_layout(object, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(object, LV_FLEX_FLOW_COLUMN);
}

List::List()
    : List(ListParamsBuilder())
{
}

void List::render()
{
}

void List::addSeparator()
{
    static auto lineParams = DividerParamsBuilder().height(1);
    if (_children->getChildrenQuantity())
    {
        Widget::addChild(std::make_shared<Divider>(lineParams));
    }
}

std::shared_ptr<Text> List::addText(std::string text)
{
    return addText(std::move(text), TextParamsBuilder());
}

std::shared_ptr<Text> List::addText(std::string text, const TextParamsBuilder &params)
{
    addSeparator();
    auto textParams = params;
    textParams.padding(Padding(5));
    auto textWidget = std::make_shared<Text>(std::move(text), textParams);
    Widget::addChild(textWidget);
    return textWidget;
}

std::shared_ptr<Button> List::addButton(std::string text)
{
    return addButton(std::move(text), ButtonParamsBuilder());
}

std::shared_ptr<Button> List::addButton(std::string text, const ButtonParamsBuilder &params)
{
    addSeparator();
    auto buttonParams = params;
    buttonParams.widthPctOfParent(100);
    buttonParams.radius(0);
    auto buttonWidget = std::make_shared<Button>(std::move(text), buttonParams);
    Widget::addChild(buttonWidget);
    return buttonWidget;
}

} // namespace lvglplus
} // namespace softeq