#include <gtest/gtest.h>
#include <memory>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>
#include <algorithm>

#include <communication/streams/include/stream_event_loop.hh>
#include <communication/streams/include/stream_controller.hh>
#include <communication/thread_blocker/include/thread_blocker.hh>

using namespace softeq::streams;
using namespace softeq::thread_blocker;

namespace
{
template <typename T>
T random(T range_from, T range_to)
{
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<T> distr(range_from, range_to);
    return distr(generator);
}
template <typename T>
class List : public std::list<T>
{
public:
    bool contains(T val) { return (std::find(this->begin(), this->end(), val) != this->end()); }
    std::string toString()
    {
        std::string st{"["};
        for (auto el = this->begin(); el != this->end(); ++el)
        {
            st += std::to_string(*el);
            st += ", ";
        }
        st += "]";
        return st;
    }
};

} // namespace

TEST(StreamTest, StreamAddListener)
{
    auto eventLoop = std::make_shared<StreamEventLoop>();
    StreamController<bool> mySt(eventLoop);
    ASSERT_FALSE(mySt.hasListener());
    auto subscription = mySt.stream()->listen([](bool) {});
    ASSERT_TRUE(mySt.hasListener());
}

struct UserDataClass
{
    UserDataClass(int x, double y)
        : x(x)
        , y(y)
    {
    }
    int x;
    double y;
    bool operator==(UserDataClass &other) const { return ((this->x == other.x) && (this->y == other.y)); }
};

TEST(StreamTest, StreamInteropInSingleThread)
{
    UserDataClass testVal{7, 9.5};
    UserDataClass resVal{0, 0.0};
    auto eventLoop = std::make_shared<StreamEventLoop>();
    StreamController<UserDataClass> mySt(eventLoop);
    ThreadBlocker threadBlocker;

    auto subscription = mySt.stream()->listen(
        [&](UserDataClass recVal)
        {
            resVal = recVal;
            threadBlocker.unblock();
        });
    mySt.add(testVal);
    threadBlocker.await();
    ASSERT_TRUE(testVal == resVal);
}

TEST(StreamTest, StreamCancelSubTwice)
{
    auto eventLoop = std::make_shared<StreamEventLoop>();
    StreamController<int> mySt(eventLoop);
    auto sub1 = mySt.stream()->listen([&](int) {});
    sub1->cancel();
    ASSERT_NO_THROW(sub1->cancel());
}

TEST(StreamTest, StreamAddHandlersWithoutBroadcast)
{
    auto eventLoop = std::make_shared<StreamEventLoop>();
    StreamController<int> mySt(eventLoop);
    auto sub1 = mySt.stream()->listen([&](int) {});
    ASSERT_THROW(auto sub2 = mySt.stream()->listen([&](int) {}), std::runtime_error);
}

TEST(StreamTest, StreamAddHandlersWithBroadcast)
{
    auto eventLoop = std::make_shared<StreamEventLoop>();
    StreamController<int> mySt(eventLoop);
    mySt.enableBroadcast();
    auto k1 = mySt.stream()->listen([&](int) {});
    ASSERT_EQ(mySt.stream()->getSubscribersQuantity(), 1);
    auto k2 = mySt.stream()->listen([&](int) {});
    ASSERT_EQ(mySt.stream()->getSubscribersQuantity(), 2);
}

TEST(StreamTest, StreamInteropHandlersWithBroadcast) // Add and cancel subs and check quantity and handlers
{
    int counterOfEvents1 = 0;
    int counterOfEvents2 = 0;
    int valueToAdd = 1;

    auto eventLoop = std::make_shared<StreamEventLoop>();
    StreamController<int> mySt(eventLoop);
    mySt.enableBroadcast();
    std::list<std::shared_ptr<StreamSubscription<int>>> listOfSubscriptions;

    ThreadBlocker threadBlocker;

    // Create subscriber 1, trigger an  event, counter1 increments when an event is caught
    listOfSubscriptions.push_back(mySt.stream()->listen(
        [&](int val)
        {
            counterOfEvents1 += 1;
            threadBlocker.unblock();
        }));
    ASSERT_EQ(mySt.stream()->getSubscribersQuantity(), 1);
    mySt.add(valueToAdd);
    threadBlocker.await();
    ASSERT_EQ(counterOfEvents1, 1);

    // Create subscriber 2, trigger an  event, counter1 and counter2 increment when an event is caught
    listOfSubscriptions.push_back(mySt.stream()->listen(
        [&](int val)
        {
            counterOfEvents2 += 1;
            threadBlocker.unblock();
        }));
    ASSERT_EQ(mySt.stream()->getSubscribersQuantity(), 2);
    mySt.add(valueToAdd);
    threadBlocker.await();
    ASSERT_EQ(counterOfEvents1, 2);
    ASSERT_EQ(counterOfEvents2, 1);

    // Close subscriber 1, trigger an event, counter2 increment when an event is caught. No more handler for counter1.
    listOfSubscriptions.pop_front();
    ASSERT_EQ(mySt.stream()->getSubscribersQuantity(), 1);
    mySt.add(valueToAdd);
    threadBlocker.await();
    ASSERT_EQ(counterOfEvents1, 2);
    ASSERT_EQ(counterOfEvents2, 2);

    // Close subscriber 2, trigger an event. No more handlers.
    listOfSubscriptions.front()->cancel();
    ASSERT_EQ(mySt.stream(), nullptr); // last subscriber will be canceled, stream will be canseled too

    // @todo think about the logic of Stream. User will receive SIGSEGV because mySt.stream() is null now
    // ASSERT_EQ(mySt.stream()->getSubscribersQuantity(), 0);

    mySt.add(valueToAdd);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ASSERT_EQ(counterOfEvents1, 2);
    ASSERT_EQ(counterOfEvents2, 2);
}

void streamListenerThread(StreamController<int> &sc, int testLength)
{
    List<int> recValues;
    ThreadBlocker threadBlocker(std::chrono::seconds(11));
    auto subscription = sc.stream()->listen(
        [&](int value)
        {
            static int curRecElemNum = 0;
            recValues.push_back(value);
            ++curRecElemNum;
            if (curRecElemNum == testLength)
            {
                for (int i = 0; i < testLength; ++i)
                {
                    ASSERT_TRUE(recValues.contains(i));
                }
                threadBlocker.unblock();
            }
        });
    ASSERT_TRUE(threadBlocker.await());
}

void dataStreamerThread(StreamController<int> &sc, int from, int to)
{
    for (int i = from; i < to; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(random(10, 100)));
        sc.add(i);
    }
}

TEST(StreamTest, StreamInteropIn2Threads)
{
    const int testLength = 80;
    const int testThreadsQuan = 2;
    std::vector<int> results;
    std::vector<std::thread> threads;
    auto eventLoop = std::make_shared<StreamEventLoop>();
    StreamController<int> mySt(eventLoop);
    std::thread listenerThread(streamListenerThread, std::ref(mySt), testLength);
    for (int i = 0; i < testThreadsQuan; i++)
    {
        threads.emplace_back(dataStreamerThread, std::ref(mySt), testLength / testThreadsQuan * i,
                             testLength / testThreadsQuan * (i + 1));
    }
    for (auto &thread : threads)
    {
        thread.join();
    }
    listenerThread.join();
}

TEST(StreamTest, StreamUnsubscribeTest)
{
    int testVal[] = {7, 55, 89};
    int resVal = 0;
    auto eventLoop = std::make_shared<StreamEventLoop>();
    StreamController<int> mySt(eventLoop);
    ThreadBlocker threadBlocker(std::chrono::milliseconds(100));

    auto subscription = mySt.stream()->listen(
        [&](int val)
        {
            resVal = val;
            threadBlocker.unblock();
        });

    ASSERT_FALSE(resVal == testVal[0]);
    // push some data
    mySt.add(testVal[0]);
    ASSERT_TRUE(threadBlocker.await());
    ASSERT_TRUE(resVal == testVal[0]);
    // push some other data
    mySt.add(testVal[1]);
    ASSERT_TRUE(threadBlocker.await());
    ASSERT_TRUE(resVal == testVal[1]);
    // cancel subscription
    subscription->cancel();
    // and push some other data: after this we shouldn't set streamReceivedData in lambda to "true"
    // and should exit from wait_for after 1 second
    mySt.add(testVal[2]);
    ASSERT_FALSE(threadBlocker.await());
    ASSERT_FALSE(resVal == testVal[2]);
}

void streamListenerThread2(StreamController<int> &sc, ThreadBlocker &mainThreadBlocker)
{
    ThreadBlocker blocker(std::chrono::seconds(10));
    auto subscription = sc.stream()->listen([&](int) { blocker.unblock(); });
    // Signaling that thread has been started and ready to accept values
    mainThreadBlocker.unblock();
    // Wait for one data portion
    bool x = blocker.await();
    ASSERT_TRUE(x);
}

TEST(StreamTest, StreamAutoUnsubscribe)
{
    auto eventLoop = std::make_shared<StreamEventLoop>();
    StreamController<int> mySt(eventLoop);
    ThreadBlocker mainThreadBlocker(std::chrono::seconds(1));
    std::thread listenerThread(streamListenerThread2, std::ref(mySt), std::ref(mainThreadBlocker));
    mainThreadBlocker.await();
    ASSERT_TRUE(mySt.hasListener());
    mySt.add(1);
    // After thread finishing, subscription should be cancelled automatically
    listenerThread.join();
    // Anyway, we shouldn't fall here
    mySt.add(2);
    // And just make sure that subscription has been cancelled
    ASSERT_FALSE(mySt.hasListener());
}

// Work in progress, this test isn't done because the Streams are not ready
// TEST(StreamTest, StreamUnsubscribeFromLambdaTest)
//{
//    auto eventLoop = std::make_shared<StreamEventLoop>();
//    StreamController<int> mySt(eventLoop);
//    bool streamReceivedData = false;
//    std::mutex _mutex;
//    std::condition_variable _streamDone;
//    StreamSubscription<int> subscription;
//    subscription = mySt.stream()->listen(
//        [&](int val)
//        {
//            std::unique_lock<std::mutex> lk(_mutex);
//            streamReceivedData = true;
//            _streamDone.notify_one();
//        });
//
//    ASSERT_TRUE(mySt.hasListener());
//    mySt.add(7);
//    std::unique_lock<std::mutex> lk(_mutex);
//    _streamDone.wait_for(lk, std::chrono::seconds(1), [&streamReceivedData] { return streamReceivedData; });
//    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
//    ASSERT_FALSE(mySt.hasListener());
//}
