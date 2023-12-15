#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <algorithm>

#include <communication/thread_blocker/include/thread_blocker.hh>

using namespace softeq::thread_blocker;

void threadFunc1(ThreadBlocker &threadBlocker)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    threadBlocker.unblock();
}

TEST(ThreadBlockerTest, SuccessUnblock)
{
    ThreadBlocker threadBlocker(std::chrono::milliseconds(200));
    auto t1 = std::chrono::high_resolution_clock::now();
    std::thread thread(threadFunc1, std::ref(threadBlocker));
    ASSERT_TRUE(threadBlocker.await());
    auto t2 = std::chrono::high_resolution_clock::now();
    ASSERT_TRUE(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1) < std::chrono::milliseconds(200));
    ASSERT_FALSE(threadBlocker.unblockedByTimeout());

    thread.join();
}

TEST(ThreadBlockerTest, UnsuccessUnblock)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    ThreadBlocker threadBlocker(std::chrono::milliseconds(200));
    ASSERT_FALSE(threadBlocker.await());
    auto t2 = std::chrono::high_resolution_clock::now();
    ASSERT_TRUE(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1) >= std::chrono::milliseconds(200));
    ASSERT_TRUE(threadBlocker.unblockedByTimeout());
}

void threadFunc2(ThreadBlocker &threadBlocker, int reuseTimes)
{
    for (int i = 0; i < reuseTimes; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        threadBlocker.unblock();
    }
}

TEST(ThreadBlockerTest, ReuseTheSameBlocker)
{
    int reuseTimes = 3;
    ThreadBlocker threadBlocker(std::chrono::milliseconds(200));
    std::thread thread(threadFunc2, std::ref(threadBlocker), reuseTimes);
    for (int i = 0; i < reuseTimes; ++i)
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        ASSERT_TRUE(threadBlocker.await());
        auto t2 = std::chrono::high_resolution_clock::now();
        ASSERT_TRUE(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1) < std::chrono::milliseconds(200));
        ASSERT_FALSE(threadBlocker.unblockedByTimeout());
    }
    thread.join();
}

void threadFunc3(ThreadBlocker &threadBlocker)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    threadBlocker.unblock();
}

TEST(ThreadBlockerTest, InfinityBlock)
{
    ThreadBlocker threadBlocker;
    auto t1 = std::chrono::high_resolution_clock::now();
    std::thread thread(threadFunc3, std::ref(threadBlocker));
    ASSERT_TRUE(threadBlocker.await());
    auto t2 = std::chrono::high_resolution_clock::now();
    ASSERT_TRUE(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1) >= std::chrono::milliseconds(200));
    ASSERT_FALSE(threadBlocker.unblockedByTimeout());
    thread.join();
}
