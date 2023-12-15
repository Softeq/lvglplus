#ifndef TEST_STREAM_H
#define TEST_STREAM_H

#include <algorithm>
#include <communication/streams/include/stream_base.hh>
#include <communication/streams/include/stream_subscription.hh>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <utility>

namespace softeq
{
namespace streams
{

template <typename T>
class Stream : public StreamBase
{
public:
    explicit Stream(VoidCallback cancellationToken, std::recursive_mutex &mutex)
        : _cancellationToken(std::move(cancellationToken))
        , _mutex(mutex){};

    std::shared_ptr<StreamSubscription<T>> listen(std::function<void(T)> onData)
    {
        std::lock_guard<std::recursive_mutex> locker(_mutex);
        if ((!allowBroadcast) && (!_streamUserSubscriptions.empty()))
        {
            throw std::runtime_error("stream is already listening");
        }
        auto subscription =
            std::make_shared<StreamSubscription<T>>(onData, [this](StreamSubscription<T> *pointerToExclude) {
                _streamUserSubscriptions.remove_if([&pointerToExclude](const std::weak_ptr<StreamSubscription<T>> &p) {
                    auto sub = p.lock();
                    if (!sub)
                    {
                        return true;
                    }
                    return sub.get() == pointerToExclude;
                });
                if (_streamUserSubscriptions.empty())
                {
                    _cancellationToken();
                }
            });
        _streamUserSubscriptions.push_back(subscription);
        return subscription;
    }

    void enableBroadcast()
    {
        allowBroadcast = true;
    }

    bool notifyListener() override
    {
        if (_mutex.try_lock())
        {
            auto curData = data.front();
            if (!_streamUserSubscriptions.empty())
            {
                for (auto sub : _streamUserSubscriptions) // std::list<std::weak_ptr<StreamSubscription<T>>>
                {
                    std::weak_ptr<StreamSubscription<T>> t;
                    if (auto validSub = sub.lock())
                    {
                        validSub->onDataHandler(curData);
                    }
                }
                data.pop();
            }
            else
            {
                // erase data?
            }
            _mutex.unlock();
            return true;
        }
        return false;
    }

    bool push(const T event)
    {
        if (!_streamUserSubscriptions.empty())
        {
            data.push(event);
            return true;
        }
        return false;
    }

    void cancel() override
    {
        std::lock_guard<std::recursive_mutex> locker(_mutex);
        while (!_streamUserSubscriptions.empty())
        {
            if (auto validSub = _streamUserSubscriptions.front().lock())
            {
                validSub->cancel();
            }
            else
            {
                _streamUserSubscriptions.pop_front();
            }
        }
    }

    int getSubscribersQuantity()
    {
        return _streamUserSubscriptions.size();
    }
    bool hasListener() override
    {
        return !_streamUserSubscriptions.empty();
    }
    ~Stream() override
    {
        this->cancel();
    }

private:
    VoidCallback _cancellationToken;
    std::queue<T> data;
    std::list<std::weak_ptr<StreamSubscription<T>>> _streamUserSubscriptions;
    std::recursive_mutex &_mutex;
    bool allowBroadcast = false;
};
} // namespace streams
} // namespace softeq

#endif // TEST_STREAM_H
