#ifndef COMMON_THREAD_BLOCKER_HH
#define COMMON_THREAD_BLOCKER_HH
#include <chrono>
#include <condition_variable>
#include <mutex>

namespace softeq
{
namespace thread_blocker
{

class ThreadBlocker
{
    enum class State
    {
        waiting,
        unblockedByTimeout,
        unblockedByThread
    };

public:
    ThreadBlocker()
        : timeout(std::chrono::seconds(0)){};
    explicit ThreadBlocker(const std::chrono::duration<double> &timeout)
        : timeout(timeout)
    {
    }
    bool await();
    void unblock();
    ThreadBlocker(const ThreadBlocker &) = delete;
    ThreadBlocker(ThreadBlocker &&) = delete;
    ThreadBlocker &operator=(const ThreadBlocker &) = delete;
    ThreadBlocker &operator=(const ThreadBlocker &&) = delete;
    inline bool unblockedByTimeout() const
    {
        return state == State::unblockedByTimeout;
    }

private:
    State state{State::waiting};
    const std::chrono::duration<double> timeout;
    std::mutex mutex;
    std::condition_variable cv;
};
} // namespace thread_blocker
} // namespace softeq

#endif // COMMON_THREAD_BLOCKER_HH
