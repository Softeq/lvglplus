#ifndef LVGL_PLUS_STREAM_WIDGET_BUILDER_HH
#define LVGL_PLUS_STREAM_WIDGET_BUILDER_HH

#include "communication/streams/include/stream_controller.hh"
#include "widgets/layouts/container.hh"

namespace softeq
{
namespace lvglplus
{
using StreamWidgetBuilderParamsBuilder = ContainerParamsBuilder;

template <typename T>
class StreamWidgetBuilder : public Container
{
public:
    StreamWidgetBuilder(const T &initialData, std::shared_ptr<streams::Stream<T>> stream,
                        std::function<std::shared_ptr<Widget>(T)> builderFunc,
                        const StreamWidgetBuilderParamsBuilder &params)
        : Container(params)
    {
        builder = builderFunc;
        _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::singleChild);
        buildChild(initialData);
        streamSubscription = stream->listen([this](const T &state) { buildChild(state); });
    }
    StreamWidgetBuilder(const T &initialData, std::shared_ptr<streams::Stream<T>> stream,
                        std::function<std::shared_ptr<Widget>(T)> builderFunc)
        : StreamWidgetBuilder(initialData, stream, builderFunc, StreamWidgetBuilderParamsBuilder())
    {
    }
    ~StreamWidgetBuilder() override
    {
        streamSubscription->cancel();
    }

private:
    void buildChild(const T &state)
    {
        std::shared_ptr<Widget> newWidget = builder(state);
        if (newWidget)
        {
            if (_children->getChildrenQuantity())
            {
                Widget::replaceChild(newWidget);
            }
            else
            {
                Widget::addChild(newWidget);
            }
        }
        else
        {
            Widget::removeChild();
        }
    }
    std::function<std::shared_ptr<Widget>(T)> builder;
    std::shared_ptr<streams::StreamSubscription<T>> streamSubscription;
    std::shared_ptr<Widget> addChild(const std::shared_ptr<Widget> &newWidget) override
    {
        return nullptr;
    };
    std::shared_ptr<Widget> replaceChild(const std::shared_ptr<Widget> &newWidget,
                                         const std::shared_ptr<Widget> &oldWidget) override
    {
        return nullptr;
    };
    std::shared_ptr<Widget> removeChild(const std::shared_ptr<Widget> &childWidget) override
    {
        return nullptr;
    };
};

} // namespace lvglplus
} // namespace softeq
#endif // LVGL_PLUS_STREAM_WIDGET_BUILDER_HH
