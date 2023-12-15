#ifndef LVGL_SINGLE_CHILD_WIDGET_HH
#define LVGL_SINGLE_CHILD_WIDGET_HH

#include "widget.hh"
#include "widget_container.hh"

namespace softeq
{
namespace lvglplus
{

class SingleChildWidget : public WidgetContainer
{
public:
    SingleChildWidget() = default;
    ~SingleChildWidget() override = default;

private:
    class Iterator : public WidgetContainer::Iterator
    {
    public:
        explicit Iterator(pointer ptr)
            : m_ptr(ptr)
        {
        }
        reference operator*() const override
        {
            return *m_ptr;
        }
        pointer operator->() const override
        {
            return m_ptr;
        }
        reference operator*() override
        {
            return *m_ptr;
        }
        pointer operator->() override
        {
            return m_ptr;
        }
        Iterator &operator++() override
        {
            m_ptr++;
            return *this;
        }

    private:
        pointer m_ptr;
    };

    std::unique_ptr<WidgetContainer::Iterator> begin() override
    {
        return std::unique_ptr<Iterator>(new Iterator(&(*_child)));
    }

    std::unique_ptr<WidgetContainer::Iterator> end() override
    {
        return std::unique_ptr<Iterator>(new Iterator(&(*_child) + 1));
    }

    /// *** ADD CHILD SECTION ***
    std::shared_ptr<Widget> add(std::shared_ptr<Widget> widget) override
    {
        _child = widget;
        return _child;
    }

    std::shared_ptr<Widget> replace(const std::shared_ptr<Widget> &newWidget,
                                    const std::shared_ptr<Widget> &oldWidget) override
    {
        _child.reset();
        _child = newWidget;
        return newWidget;
    }

    std::shared_ptr<Widget> remove(const std::shared_ptr<Widget> &childWidget) override
    {
        _child.reset();
        return childWidget;
    }

    unsigned short getChildrenQuantity() override
    {
        return _child ? 1 : 0;
    }

    std::shared_ptr<Widget> _child = nullptr;
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_SINGLE_CHILD_WIDGET_HH
