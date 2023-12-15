# Stream <a name="stream"></a>

A Stream provides a way to receive a sequence of events.
Each event is either a data event, also called an element of the stream.
Streams allow you to work with data sequence in functional style.

# Motivation <a name="motivation"></a>

Streams allow you to set a data trap long before the first data appears in the stream
and send events (push data into the Stream) when it's necessary.
If the trap is set, the data will be transferred to it after some time.

- Extremely simple interface: all you need is `StreamController` and trap function
- There is no need to open a socket, file, pipe or explicitly use shared memory
- Unlike Java or Dart Streams this version is thread safe: you can send data from thread to thread

# Index <a name="index"></a>

- [Motivation](#motivation)
- [Index](#index)
- [How to use](#how-to-use)
    - [Install](#install)
    - [Creating a StreamController](#creating-a-streamcontroller)
    - [Stream listening](#stream-listening)
    - [Add data to the Stream](#add-data-to-the-stream)
- [Limitations](#limitations)

# How to use <a name="how-to-use"></a>

## Install <a name="install"></a>

To add `Streams` into your project, please use the root instruction.

## Creating a StreamController <a name="creating-a-streamcontroller"></a>

An example is better than a long abstract explanation, so let us guide you step by step:

First of all you should add this includes:

```c++
#include <communication/streams/include/stream_event_loop.hh>
#include <communication/streams/include/stream_controller.hh>
```

Don't forget that Streams are encapsulated inside the corresponding namespace.

Before we start, you should know that Streams are served by event loop called `StreamEventLoop` in dedicated thread.
So, it's necessary to create this loop and pass it to the created `StreamController`.
One `StreamEventLoop` object can serve many `StreamController` objects.
Moreover, typically you will have only one `StreamEventLoop` in your program.

***IMPORTANT:*** `StreamEventLoop` shouldn't be destroyed during any served streams are in use.

When you're creating `StreamController` you must specify *transmitting data type*.
In our example let it be `int`:

```c++
void foo()
{
    // ...
    auto eventLoop = std::make_shared<StreamEventLoop>();
    StreamController<int> streamController(eventLoop);
    // ...
}
```


## Stream listening <a name="stream-listening"></a>


Once you have `StreamController` object, you can get the associated `Stream`.
This object is creating inside the `StreamController`, so you can get it anywhere,
also in different thread. All you need in this case - just pass link to the `StreamController`.

We can `listen` stream object. This method receives a lambda or function object, which will be called on every data
event.
It your trap for the data, and you can (but not obliged) setup it long before first [add](#add-data-to-the-stream) call.

```c++
void threadFunc(StreamController<int> &streamController)
{
    auto subscription = streamController.stream()->listen(
        [&](int value)
        {
            /* your code */
        });
}
```

- All data inside your trap function will be processed in separate thread.
  ***If you capture any data from the context - you must care about thread-safety of this data.***
- Type of data inside your function would be the same as the `StreamController` data type.
- `listen` returns `StreamSubscription` object and it's necessary to get it.
  Read more about [StreamSubscription](#stream-subscription).

In case if you will transmit `StreamController` object to another thread,
please, don't forget that you should pass it by reference using *std::ref*:

```c++
void foo()
{
    // other code and creating a StreamController<int>
    std::thread listenerThread(threadFunc, std::ref(streamController));
    // ...
}
```

## Stream subscription <a name="stream-subscription"></a>

Once you've call `listen` method on `Stream`, you'll get a `StreamSubscription` object.

`StreramSubscription` is used to cancel subscription if you don't want to receive
new events from the `Stream`. Once you cancel subscription, you wouldn't be able
to resume it. 

You can use `std::move` to move subscription, but you can't copy it.

***IMPORTANT:*** you **should** get this object.
It's necessary, because C++ doesn't allow us to get runtime information about trap function
if it's lambda: is it still alive? If you got `StreamSubscription` object, your `Stream`
will be closed automatically while this object destroying.
Now it's impossible to cancel subscription inside a trap function.

For example, you can realise logic like that:

```c++
void threadFunc(StreamController<int> streamController)
    std::mutex _mutex;
    bool streamReceivedData = false;
    std::condition_variable _streamDone;
    int dataRecTimes = 0;
    
    auto subscription = streamController.stream()->listen(
        [&](int data)
        {
            ++dataRecTimes;
            if (dataRecTimes == 10)
            {
                std::lock_guard<std::mutex> lockRecData(_mutex);
                streamReceivedData = true;
                _streamDone.notify_one();
            }
        });
    std::unique_lock<std::mutex> lk(_mutex);
    _streamDone.wait(lk, [&streamReceivedData] { return streamReceivedData; });
    subscription.cancel();
    // do some action
}
```
In this case we're waiting for 10 int events and after this cancel the subscription.
This thread is waiting conditional variable which would be set inside the trap function.
After passing `wait` condition we can do some other actions in this thread.

**Useful tools:** you can use `ThreadBlocker` to reduce
code lines and increase readability of code. Let's rewrite this example using `ThreadBlocker`:
```c++
// ...
#include <communication/thread_blocker/include/thread_blocker.hh>
// ...
void threadFunc(StreamController<int> streamController)
    ThreadBlocker threadBlocker; // blocker with infinity wait
    int dataRecTimes = 0;
    
    auto subscription = streamController.stream()->listen(
        [&](int data)
        {
            ++dataRecTimes;
            if (dataRecTimes == 10)
            {
                threadBlocker.unblock(); // blocker unblock
            }
        });
    threadBlocker.await(); // infinity wait
    subscription.cancel();
    // do some action
}
```
## Add data to the Stream <a name="add-data-to-the-stream"></a>
It's the simplest part of using `Streams`. All what you need is associated `StreamController`:
just call `add` function and pass new data event. This object would be copied.
```c++
void foo()
{
    streamController.add(7);
    streamController.add(5);
}
```
## Limitations <a name="limitations"></a>
Current implementation has some limitations, which you should to keep in mind:

- You can't cancel `StreamSubscription` inside a trap function
- You can't pause/resume `Stream`