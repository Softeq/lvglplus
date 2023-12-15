#ifndef LVGL_PLUS_BLOC_BASE_HH
#define LVGL_PLUS_BLOC_BASE_HH

#include <communication/streams/include/stream_controller.hh>

namespace softeq
{
namespace lvglplus
{

template <typename State>
class BlocBase
{
public:
    template <typename SubState, typename = typename std::enable_if<std::is_base_of<State, SubState>::value>::type>
    explicit BlocBase(SubState &&state)
    {
        streamEventLoop = std::make_shared<streams::StreamEventLoop>();
        stateStreamController = std::make_shared<streams::StreamController<std::shared_ptr<State>>>(streamEventLoop);
        _state = std::shared_ptr<State>(new SubState(std::forward<SubState>(state)));
    }

    std::shared_ptr<State> state()
    {
        return _state;
    }

    std::shared_ptr<streams::Stream<std::shared_ptr<State>>> getStateStream()
    {
        return stateStreamController->stream();
    }

    template <typename SubState>
    std::shared_ptr<SubState> getIf(const std::shared_ptr<State> &state)
    {
        return std::dynamic_pointer_cast<SubState>(state);
    }

protected:
    void emit(std::shared_ptr<State> state)
    {
        _state = state;
        stateStreamController->add(state);
    }
    std::shared_ptr<streams::StreamEventLoop> streamEventLoop;

private:
    std::shared_ptr<State> _state;
    std::shared_ptr<streams::StreamController<std::shared_ptr<State>>> stateStreamController;
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_PLUS_BLOC_BASE_HH
