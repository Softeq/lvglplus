# Thread Blocker <a name="thread-blocker"></a>

This tool provides you easy way to work with threads in async manner.
With `ThreadBlocker` you can block one thread and wait until other thread 
unblock it.

# Motivation <a name="motivation"></a>

Sometimes you'll need to block your current thread until something happens in others.
You can implement this logic using [condition variable](https://en.cppreference.com/w/cpp/thread/condition_variable),
[mutex](https://en.cppreference.com/w/cpp/thread/mutex) and a `bool` variable.

Let's see the example:

```c++
#include <chrono>
#include <mutex>
#include <condition_variable>

void threadFunc(std::condition_variable &cv, std::mutex &mutex, bool &done)
{
    /* do some work */
    std::lock_guard<std::mutex> lock(mutex);
    done = true;
    cv.notify_one();
    /* do other work */
}

int main()
{
    /* do some work */
    std::mutex mutex;
    std::condition_variable cv;
    bool done = false;
    std::thread thread(threadFunc, std::ref(cv), std::ref(mutex), std::ref(done));
    std::unique_lock<std::mutex> lk(mutex);
    cv.wait(lk, [&] { return done; }); // or you can use 'wait_for'
    /* code, which should be called only after conditional variable notification */
}
```

So, we had to create 3 different variables, pass all of them to thread and do a lot of manual work.

Let's rewrite this example with `ThreadBlocker`:

```c++
#include <communication/thread_blocker/include/thread_blocker.hh>
using namespace softeq::thread_blocker;

void threadFunc(ThreadBlocker& threadBlocker)
{
    /* do some work */
   threadBlocker.unblock();
    /* do other work */
}

int main()
{
    /* do some work */
    ThreadBlocker threadBlocker;
    std::thread thread(threadFunc, std::ref(threadBlocker));
    threadBlocker.await();
    /* code, which should be called only after conditional variable notification */
}
```

Thus, you don't have to create a lot of variables. Moreover, you can set `timeot` for `ThreadBlocker`.

# Index <a name="index"></a>

- [Motivation](#motivation)
- [Index](#index)
- [How to use](#how-to-use)
    - [Install](#install)
    - [Creating a ThreadBlocker](#creating-a-threadblocker)
      - [Blocker with timeout](#blocker-with-timeout)
      - [Infinity blocker](#infinity-blocker)
    - [Blocker waiting](#blocker-waiting)
    - [Unblock a blocker](#unblock-a-blocker)
    - [Reuse a blocker](#reuse-a-blocker)

# How to use <a name="how-to-use"></a>

## Install <a name="install"></a>

To add `ThreadBlocker` into your project, please use the root instruction.

## Creating a ThreadBlocker <a name="creating-a-threadblocker"></a>

You have 2 options for creating a ThreadBlocker:

- [blocker with timeout](#blocker-with-timeout)
- [infinity blocker](#infinity-blocker)

### Blocker with timeout <a name="blocker-with-timeout"></a>

It's the most common way to create blocker. All what you need is to pass 
`std::chrono` object into constructor: it might be both seconds or milliseconds.
Once you set timeout for a `ThreadBlocker` it can't be changed.

This blocker will wait until someone calls `unblock()` or until `timeout` runs out.
You will be able to [find out on which event the blocking state was exited.](#check-unblocked-by-timeout)

```c++
#include <communication/thread_blocker/include/thread_blocker.hh>
using namespace softeq::thread_blocker;

void foo()
{
    ThreadBlocker threadBlocker(std::chrono::seconds(1));
    
}
```

### Infinity blocker <a name="infinity-blocker"></a>



```c++
#include <communication/thread_blocker/include/thread_blocker.hh>
using namespace softeq::thread_blocker;

void foo()
{
    ThreadBlocker threadBlocker(std::chrono::seconds(1));
    
}
```

## Blocker waiting <a name="blocker-waiting"></a>

You can block current thread by calling `await()` function on 
`ThreadBlocker` object. 

```c++
ThreadBlocker threadBlocker;
/* some code */
threadBlocker.await();
```
<a name="check-unblocked-by-timeout"></a>
You can check if the `ThreadBlocker` was unblocked by timeout or not 
by checking the return value of `await` function: 

```c++
ThreadBlocker threadBlocker;
/* some code */
if (threadBlocker.await())
    std::cout << "Unblocked by other thread" << std::endl;
else
    std::cout << "Unblocked by timeout" << std::endl;
```
Or you can get this information by calling `unblockedByTimeout()`:
```c++
ThreadBlocker threadBlocker;
/* some code */
threadBlocker.await();
if (threadBlocker.unblockedByTimeout())
    std::cout << "Unblocked by other thread" << std::endl;
else
    std::cout << "Unblocked by timeout" << std::endl;
```

## Unblock a blocker <a name="unblock-a-blocker"></a>

Just call `unblock()` function on `ThreadBlocker` object in any non-blocked by this blocker thread.

```c++
threadBlocker.unblock();
```

## Reuse a blocker <a name="reuse-a-blocker"></a>
It's absolutely ok to reuse one `ThreadBlocker` object many times:


```c++
void threadFunc(ThreadBlocker& threadBlocker)
{
    /* do some work */
   threadBlocker.unblock();
    /* do else work */
   threadBlocker.unblock();
   /* do other work */
}

int main()
{
    /* do some work */
    ThreadBlocker threadBlocker;
    std::thread thread(threadFunc, std::ref(threadBlocker));
    threadBlocker.await();
    /* do some work */
    threadBlocker.await();
    /* do some work */
}
```
This example is correct, but you should keep in mind that if `unblock()` was called before
`await()`, your code will be totally blocked in case if you're using `infinity blocker`.