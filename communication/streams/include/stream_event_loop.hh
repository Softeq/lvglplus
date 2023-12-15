#ifndef LVGL_PLUS_STREAM_EVENT_LOOP_H
#define LVGL_PLUS_STREAM_EVENT_LOOP_H

#include "list"
#include <communication/streams/include/stream.hh>
#include <condition_variable>
#include <mutex>
#include <thread>


namespace softeq
{
namespace streams
{

enum class LoopState
{
    Initializing,
    Working,
    Idle,
    Finishing
};

class StreamEventLoop
{
public:
    StreamEventLoop(const StreamEventLoop &) = delete;
    StreamEventLoop &operator=(StreamEventLoop &) = delete;

    void scheduleStreamProcess(std::shared_ptr<StreamBase> stream);
    StreamEventLoop();
    ~StreamEventLoop();

private:
    void EventLoopThread();
    std::unique_ptr<std::thread> _loopThread;
    std::queue<std::shared_ptr<StreamBase>> _streamsToProcessQueue;
    std::mutex _streamsToProcessQueueMutex;
    std::condition_variable _continueLoopCV;
    std::mutex _loopStateMutex;
    LoopState _loopState = LoopState::Initializing;
    std::mutex _createLoopMutex;
    std::condition_variable _createLoopCV;
};
} // namespace streams
} // namespace softeq

#endif // LVGL_PLUS_STREAM_EVENT_LOOP_H
