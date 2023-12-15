#ifndef TEST_STREAM_CONTROLLER_H
#define TEST_STREAM_CONTROLLER_H

#include <communication/streams/include/stream.hh>
#include <communication/streams/include/stream_event_loop.hh>
#include <communication/streams/include/stream_sink.hh>
#include <condition_variable>
#include <mutex>
#include <utility>

namespace softeq
{
namespace streams
{

template <typename T>
class StreamController : public StreamSink<T>
{
public:
    explicit StreamController(std::shared_ptr<StreamEventLoop> eventLoop)
        : _streamEventLoop(std::move(eventLoop))
    {
        std::lock_guard<std::recursive_mutex> lock(_controlledStreamMutex);
        _controlledStream.reset(new Stream<T>(
            [this]() {
                std::lock_guard<std::recursive_mutex> controlledStreamLocker(_controlledStreamMutex);
                _controlledStream.reset();
            },
            _controlledStreamMutex));
    }

    void add(T event) override
    {
        std::lock_guard<std::recursive_mutex> lock(_controlledStreamMutex);
        if (_controlledStream != nullptr)
        {
            auto stream = _controlledStream;
            if (stream->push(event))
            {
                _streamEventLoop->scheduleStreamProcess(_controlledStream);
            }
        }
    }

    std::shared_ptr<Stream<T>> stream()
    {
        return _controlledStream;
    }
    void enableBroadcast()
    {
        _controlledStream->enableBroadcast();
    }

    bool hasListener()
    {
        std::lock_guard<std::recursive_mutex> lock(_controlledStreamMutex);
        return _controlledStream != nullptr && _controlledStream->hasListener();
    }
    ~StreamController() = default;

private:
    std::shared_ptr<StreamEventLoop> _streamEventLoop;
    std::shared_ptr<Stream<T>> _controlledStream;
    std::recursive_mutex _controlledStreamMutex;
};
} // namespace streams
} // namespace softeq

#endif // TEST_STREAM_CONTROLLER_H
