#ifndef LVGL_PLUS_WIDGET_CONTAINER_HH
#define LVGL_PLUS_WIDGET_CONTAINER_HH

#include <memory>

namespace softeq
{
namespace lvglplus
{
enum class WidgetContainerType
{
    multiChild,
    singleChild,
    noChild
};

class Widget;

class WidgetContainer
{
public:
    class Iterator
    {
    public:
        using value_type = Widget;
        using pointer = value_type *;
        using reference = value_type &;

        virtual reference operator*() const = 0;
        virtual pointer operator->() const = 0;
        virtual reference operator*() = 0;
        virtual pointer operator->() = 0;
        virtual Iterator &operator++() = 0;
        bool operator==(const Iterator &other) const
        {
            return operator->() == other.operator->();
        }
        bool operator!=(const Iterator &other) const
        {
            return !operator==(other);
        };
        virtual ~Iterator() = default;
    };
    virtual std::unique_ptr<Iterator> begin() = 0;
    virtual std::unique_ptr<Iterator> end() = 0;

    virtual std::shared_ptr<Widget> add(std::shared_ptr<Widget> childWidget) = 0;
    virtual std::shared_ptr<Widget> replace(const std::shared_ptr<Widget> &newWidget,
                                            const std::shared_ptr<Widget> &oldWidget) = 0;
    virtual std::shared_ptr<Widget> remove(const std::shared_ptr<Widget> &childWidget) = 0;
    virtual ~WidgetContainer() = default;
    static std::unique_ptr<WidgetContainer> MakeWidgetContainer(WidgetContainerType widgetContainerType);
    virtual unsigned short getChildrenQuantity() = 0;
};
} // namespace lvglplus
} // namespace softeq

#endif // LVGL_PLUS_WIDGET_CONTAINER_HH
