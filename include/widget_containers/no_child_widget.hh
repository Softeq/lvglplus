#ifndef LVGL_PLUS_NO_CHILD_WIDGET_HH
#define LVGL_PLUS_NO_CHILD_WIDGET_HH

#include "widget.hh"
#include "widget_container.hh"

namespace softeq
{
namespace lvglplus
{

class NoChildWidget : public WidgetContainer
{
public:
    NoChildWidget() = default;
    ~NoChildWidget() override = default;

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
            return *this;
        }

    private:
        pointer m_ptr;
    };
    std::unique_ptr<WidgetContainer::Iterator> begin() override
    {
        return std::unique_ptr<Iterator>(new Iterator(nullptr));
    }
    std::unique_ptr<WidgetContainer::Iterator> end() override
    {
        return std::unique_ptr<Iterator>(new Iterator(nullptr));
    }

    /// *** ADD CHILD SECTION ***
    std::shared_ptr<Widget> add(std::shared_ptr<Widget> widget) override
    {
        // static_assert(false, "Can't add a child to a child-free widget"); // @todo raise an error
        return nullptr;
    }

    std::shared_ptr<Widget> replace(const std::shared_ptr<Widget> &newWidget,
                                    const std::shared_ptr<Widget> &oldWidget) override
    {
        return newWidget;
    }

    std::shared_ptr<Widget> remove(const std::shared_ptr<Widget> &childWidget) override
    {
        return nullptr;
    }

    unsigned short getChildrenQuantity() override
    {
        return 0;
    }
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_PLUS_NO_CHILD_WIDGET_HH
