#ifndef TEST_STREAM_SUBSCRIPTION_H
#define TEST_STREAM_SUBSCRIPTION_H

#include <atomic>
#include <communication/streams/include/stream_base.hh>
#include <utility>

namespace softeq
{
namespace streams
{

/// A subscription on events from a [Stream].
template <typename T>
class StreamSubscription
{
public:
    explicit StreamSubscription(std::function<void(T)> onData,
                                std::function<void(StreamSubscription<T> *)> cancelCallback)
        : onDataHandler(onData)
        , cancelCallback(cancelCallback)
    {
    }

    // We use this constructor to carefully transmit prepared subscription object
    // and store it until someone call copy constructor
    StreamSubscription(StreamSubscription &&subscription) noexcept
        : onDataHandler(subscription.onDataHandler)
        , cancelCallback(subscription.cancelCallback)
    {
        subscription.onDataHandler = nullptr;
        subscription.cancelCallback = nullptr;
    }

    StreamSubscription(const StreamSubscription &subscription) = delete;

    StreamSubscription &operator=(StreamSubscription &&subscription) = delete;

    StreamSubscription &operator=(const StreamSubscription &) = delete;

    ~StreamSubscription()
    {
        cancel();
    }

    /// Cancels this subscription.
    void cancel()
    {
        if (cancelCallback)
        {
            cancelCallback(this);
            cancelCallback = nullptr;
        }
    }

    std::function<void(T)> onDataHandler = nullptr;

private:
    std::function<void(StreamSubscription<T> *)> cancelCallback = nullptr;
};
} // namespace streams
} // namespace softeq

#endif // TEST_STREAM_SUBSCRIPTION_H
