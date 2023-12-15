#ifndef LVGL_PLUS_BLOC_HH
#define LVGL_PLUS_BLOC_HH

#include "bloc_base.hh"
#include "communication/streams/include/stream_controller.hh"
#include <memory>

namespace softeq
{
namespace lvglplus
{

template <typename Event, typename State>
class Bloc : public BlocBase<State>
{
public:
    template <typename SubState, typename = typename std::enable_if<std::is_base_of<State, SubState>::value>::type>
    explicit Bloc(SubState &&initialState)
        : BlocBase<State>(std::forward<SubState>(initialState))
    {
        eventStreamController =
            std::make_shared<streams::StreamController<std::shared_ptr<Event>>>(this->streamEventLoop);
        eventStreamController->enableBroadcast();
    }

    template <typename SubEvent, typename = typename std::enable_if<std::is_base_of<Event, SubEvent>::value>::type>
    void add(SubEvent &&event)
    {
        auto sharedEvent = std::shared_ptr<Event>(new SubEvent(event));
        eventStreamController->add(sharedEvent);
    }

    template <typename SubEvent, typename = typename std::enable_if<std::is_base_of<Event, SubEvent>::value>::type>
    void on(std::function<void(SubEvent)> &&handler)
    {
        eventStreamSubscriptions.push_back(
            eventStreamController->stream()->listen([handler](std::shared_ptr<Event> event) {
                std::shared_ptr<SubEvent> subEvent = std::dynamic_pointer_cast<SubEvent>(event);
                if (subEvent)
                {
                    handler(*subEvent);
                }
            }));
    }

    template <typename SubState, typename = typename std::enable_if<std::is_base_of<State, SubState>::value>::type>
    void emit(SubState &&state)
    {
        auto sharedState = std::shared_ptr<State>(new SubState(state));
        BlocBase<State>::emit(sharedState);
    }

    std::shared_ptr<streams::Stream<Event>> getEventStream()
    {
        return eventStreamController->stream();
    }

private:
    std::shared_ptr<streams::StreamController<std::shared_ptr<Event>>> eventStreamController;
    std::list<std::shared_ptr<streams::StreamSubscription<std::shared_ptr<Event>>>> eventStreamSubscriptions;
};
} // namespace lvglplus
} // namespace softeq

#endif // LVGL_PLUS_BLOC_HH
