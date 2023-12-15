#include "communication/thread_blocker/include/thread_blocker.hh"

using namespace softeq::thread_blocker;

bool ThreadBlocker::await()
{
    state = State::waiting;
    std::unique_lock<std::mutex> lk(mutex);
    if (timeout != std::chrono::seconds(0))
    {
        if (cv.wait_for(lk, timeout, [&] { return state == State::unblockedByThread; }))
        {
            return true;
        }
        state = State::unblockedByTimeout;
        return false;
    }
    cv.wait(lk, [&] { return state == State::unblockedByThread; });
    return true;
}
void ThreadBlocker::unblock()
{
    std::lock_guard<std::mutex> lock(mutex);
    state = State::unblockedByThread;
    cv.notify_one();
}