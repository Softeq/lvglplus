#include "wait_for.hh"
#include <thread>

using namespace softeq::lvglplus;

bool waitForCondition(const std::function<bool()> &condition, int timeoutCounterMs)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    while (!condition())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count() >= timeoutCounterMs)
        {
            return false;
        }
    }
    return true;
}

bool waitForRender(std::shared_ptr<Widget> widget)
{
    return waitForCondition([&]() -> bool { return widget->isRendered(); });
}