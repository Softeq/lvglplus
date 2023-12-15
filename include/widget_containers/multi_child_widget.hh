#ifndef LVGL_MULTI_CHILD_WIDGET_HH
#define LVGL_MULTI_CHILD_WIDGET_HH

#include "widget.hh"
#include "widget_container.hh"
#include <algorithm>
#include <list>

namespace softeq
{
namespace lvglplus
{

class MultiChildWidget : public WidgetContainer
{
public:
    MultiChildWidget() = default;
    ~MultiChildWidget() override
    {
        _children.clear();
    }

private:
    class Iterator : public WidgetContainer::Iterator
    {
    public:
        using intRepr = std::list<std::shared_ptr<Widget>>::iterator;
        explicit Iterator(intRepr ptr)
            : m_ptr(ptr)
        {
        }
        reference operator*() const override
        {
            return **m_ptr;
        }
        pointer operator->() const override
        {
            return &(**m_ptr);
        }
        reference operator*() override
        {
            return **m_ptr;
        }
        pointer operator->() override
        {
            return &(**m_ptr);
        }
        Iterator &operator++() override
        {
            m_ptr++;
            return *this;
        }

    private:
        intRepr m_ptr;
    };

    std::unique_ptr<WidgetContainer::Iterator> begin() override
    {
        return std::unique_ptr<Iterator>(new Iterator(_children.begin()));
    }

    std::unique_ptr<WidgetContainer::Iterator> end() override
    {
        return std::unique_ptr<Iterator>(new Iterator(_children.end()));
    }

    /// *** ADD CHILD SECTION ***
    std::shared_ptr<Widget> add(std::shared_ptr<Widget> widget) override
    {
        _children.push_back(widget);
        _lastUsedWidget = widget;
        return widget;
    }

    std::shared_ptr<Widget> replace(const std::shared_ptr<Widget> &newWidget,
                                    const std::shared_ptr<Widget> &oldWidget) override
    {
        if (!oldWidget)
        {
            remove(nullptr);
            add(newWidget);
        }
        else
        {
            newWidget->setParent(oldWidget->parent());
            auto it = std::find(_children.begin(), _children.end(), oldWidget);
            auto pos = _children.erase(it);
            _children.insert(pos, newWidget);
            static_cast<Renderable *>(newWidget.get())->renderWithChildren();
        }
        return newWidget;
    }

    std::shared_ptr<Widget> remove(const std::shared_ptr<Widget> &childWidget) override
    {
        if (!_children.empty())
        {
            if (childWidget)
            {
                auto it = std::find(_children.begin(), _children.end(), childWidget);
                _children.erase(it);
            }
            else
            {
                _children.clear();
            }
        }
        return childWidget;
    }
    unsigned short getChildrenQuantity() override
    {
        return _children.size();
    }

    std::list<std::shared_ptr<Widget>> _children;
    std::weak_ptr<Widget> _lastUsedWidget;
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_MULTI_CHILD_WIDGET_HH
