#include "widget_container.hh"
#include "widget_containers/multi_child_widget.hh"
#include "widget_containers/no_child_widget.hh"
#include "widget_containers/single_child_widget.hh"

namespace softeq
{
namespace lvglplus
{
std::unique_ptr<WidgetContainer> WidgetContainer::MakeWidgetContainer(WidgetContainerType widgetContainerType)
{
    switch (widgetContainerType)
    {
    case WidgetContainerType::multiChild:
        return std::unique_ptr<MultiChildWidget>(new MultiChildWidget());
    case WidgetContainerType::singleChild:
        return std::unique_ptr<SingleChildWidget>(new SingleChildWidget());
    case WidgetContainerType::noChild:
        return std::unique_ptr<NoChildWidget>(new NoChildWidget());
    }
    return nullptr;
}
} // namespace lvglplus
} // namespace softeq