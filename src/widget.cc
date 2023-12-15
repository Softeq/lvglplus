#include "widget.hh"
#include "lvgl_plus.hh"
#include "widgets/properties/implementation/widget_params.hh"
#include <iostream>
#include <memory>
#include <vector>

namespace softeq
{
namespace lvglplus
{

Widget::Widget()
    : Widget("Widget", WidgetParams())
{
}

Widget::Widget(std::string name)
    : Widget(std::move(name), WidgetParams())
{
    setBorderWidth(0);
    setPadding(Padding(0));
    setRadius(0);
    setWidth(LV_PCT(100));
    setHeight(LV_PCT(100));
}

Widget::Widget(std::string name, const WidgetParams &params)
    : Widget(std::move(name), params, lv_obj_create)
{
}

Widget::Widget(std::string name, const WidgetParams &params,
               const std::function<lv_obj_t *(lv_obj_t *parent)> &createFunc)
{
    _widgetName = std::move(name);
    if (createFunc)
    {
        UniqueLock locker(LvglPlus::lvglMutex());
        lv_obj_t *lvObject = createFunc(lv_scr_act());
        prepareWidget(params, lvObject);
    }
}

Widget::Widget(std::string name, lv_obj_t *lvObject)
{
    _widgetName = std::move(name);
    lvglObject.setObject(lvObject);
}

Widget::Widget(std::string name, const WidgetParams &params, lv_obj_t *lvObject)
{
    _widgetName = std::move(name);
    if (lvObject)
    {
        prepareWidget(params, lvObject);
    }
}

void Widget::prepareWidget(const WidgetParams &params, lv_obj_t *lvObject)
{
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::singleChild);
    _zIndex = params.zIndex;
    lvglObject.setObject(lvObject);
    if (lvObject && params.instructions)
    {
        for (const auto &instruction : *params.instructions)
        {
            instruction(lvObject);
        }
    }
}

Widget::~Widget()
{
    _children.reset();
}

lv_obj_t *Widget::getLvObjectFromParent()
{
    if (_parent)
    {
        auto object = _parent->lvglObject.getObject();
        return object ? object : _parent->getLvObjectFromParent();
    }
    else
    {
        return lv_scr_act();
    }
}

void Widget::renderChildren()
{
    bool alreadyRendering = !_rendered;
    _rendered = false;
    if (_children->getChildrenQuantity())
    {
        bool standartOrder = true;
        for (auto iterChild = _children->begin(); *iterChild != *(_children->end()); ++(*iterChild))
        {
            (*iterChild)->renderWithChildren();
            if ((*iterChild)->zIndex())
            {
                standartOrder = false;
            }
        }
        // apply layer order according to priority
        if (!standartOrder)
        {
            struct Order
            {
                int index;
                int order;
            };
            std::list<Order> k;
            int index = 0;
            for (auto iterChild = _children->begin(); *iterChild != *(_children->end()); ++(*iterChild))
            {
                k.push_back({index, (*iterChild)->zIndex()});
                index++;
            }
            // make an order from a priority
            k.sort([](const Order &a, const Order &b) -> bool { return a.order < b.order; });
            index = 0;
            for (auto &ord : k)
            {
                ord.order = index;
                index++;
            }
            k.sort([](const Order &a, const Order &b) -> bool { return a.index < b.index; });
            // apply an order
            auto iter = k.begin();
            for (auto iterChild = _children->begin(); *iterChild != *(_children->end()); ++(*iterChild))
            {
                lv_obj_move_to_index((*iterChild)->lvglObject.getObject(), iter->order);
                iter++;
            }
        }
    }
    if (!alreadyRendering)
    {
        _rendered = true;
    }
}

void Widget::renderWithChildren()
{
    _rendered = false;
    render();
    renderChildren();

    UniqueLock locker(LvglPlus::lvglMutex());
    lv_obj_update_layout(lvglObject.getObject());
    _rendered = true;
}

std::shared_ptr<Widget> Widget::removeChild(const std::shared_ptr<Widget> &childWidget)
{
    if (_children)
    {
        _children->remove(childWidget);
        if (_rendered)
        {
            renderWithChildren();
        }
    }
    return childWidget;
}

std::shared_ptr<Widget> Widget::replaceChild(const std::shared_ptr<Widget> &newWidget,
                                             const std::shared_ptr<Widget> &oldWidget)
{
    if (_children)
    {
        newWidget->setParent(this);
        _children->replace(newWidget, oldWidget);
        if (_rendered)
        {
            renderWithChildren();
        }
    }
    return newWidget;
}

std::shared_ptr<Widget> Widget::addChild(const std::shared_ptr<Widget> &newWidget)
{
    if (_children)
    {
        newWidget->setParent(this);
        _children->add(newWidget);
    }
    return newWidget;
}

void Widget::applyPadding(const Padding &padding)
{
    auto lvObject = lvglObject.getObject();
    lv_obj_set_style_pad_top(lvObject, padding.getTop(), 0);
    lv_obj_set_style_pad_bottom(lvObject, padding.getBottom(), 0);
    lv_obj_set_style_pad_left(lvObject, padding.getLeft(), 0);
    lv_obj_set_style_pad_right(lvObject, padding.getRight(), 0);
    lv_obj_set_style_pad_row(lvObject, padding.getRow(), 0);
    lv_obj_set_style_pad_column(lvObject, padding.getColumn(), 0);
}

std::string Widget::toString()
{
    std::string outputStr = "Instance of " + _widgetName;
    if (!_diagnosticName.empty())
    {
        outputStr += " (" + _diagnosticName + ")";
    }
    return outputStr;
}

const unsigned int indentWidth = 3;

std::string Widget::subtreeToString(const std::shared_ptr<Widget> &widgetToMark)
{
    return subtreeToStringRecurcive(&(*widgetToMark));
}

std::string Widget::subtreeToStringRecurcive(Widget *widgetToMark, unsigned int indentCnt)
{
    std::string treeStr;
    treeStr += toString();
    auto childrenQuantity = _children->getChildrenQuantity();
    if (childrenQuantity)
    {
        treeStr += " (amount of children - " + std::to_string(childrenQuantity) + "):";
        treeStr += (this == widgetToMark) ? " <-------- Marked widget\n" : "\n";
        std::string indentPattern = "|";
        indentPattern.append(indentWidth - 1, ' ');
        std::string indent;
        for (unsigned int i = 0; i < indentCnt; i++)
        {
            indent += indentPattern;
        }
        for (auto iterChild = _children->begin(); *iterChild != *(_children->end()); ++(*iterChild))
        {
            treeStr += indent + (*iterChild)->subtreeToStringRecurcive(widgetToMark, indentCnt + 1);
        }
    }
    else
    {
        treeStr += (this == widgetToMark) ? " <-------- Marked widget\n" : "\n";
    }
    return treeStr;
}

std::string Widget::wholeTreeToString(bool markWidget)
{
    std::string treeStr = "Widget tree:\n";
    Widget *rootParent = this;
    while (rootParent->parent())
    {
        rootParent = rootParent->parent();
    }
    treeStr += rootParent->subtreeToStringRecurcive(markWidget ? this : nullptr);

    return treeStr;
}

void Widget::render()
{
}

} // namespace lvglplus
} // namespace softeq