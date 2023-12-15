#include "communication/streams/include/stream_event_loop.hh"
#include <iostream>
#include <memory>
#include <thread>

using namespace softeq::streams;

void StreamEventLoop::EventLoopThread()
{
    while (true)
    {
        std::unique_lock<std::mutex> continueLoopLock(_loopStateMutex);
        switch (_loopState)
        {
        case LoopState::Initializing: {
            _loopState = LoopState::Idle;
            continueLoopLock.unlock();
            std::unique_lock<std::mutex> lock(_createLoopMutex);
            _createLoopCV.notify_one();
            break;
        }
        case LoopState::Working: {
            continueLoopLock.unlock();
            _streamsToProcessQueueMutex.lock();
            if (!_streamsToProcessQueue.empty())
            {
                auto stream = _streamsToProcessQueue.front();
                _streamsToProcessQueueMutex.unlock();
                if (stream->notifyListener())
                {
                    _streamsToProcessQueueMutex.lock();
                    _streamsToProcessQueue.pop();
                    _streamsToProcessQueueMutex.unlock();
                }
            }
            _streamsToProcessQueueMutex.lock();
            if (_streamsToProcessQueue.empty())
            {
                continueLoopLock.lock();
                _loopState = LoopState::Idle;
                continueLoopLock.unlock();
            }
            _streamsToProcessQueueMutex.unlock();
            break;
        }
        case LoopState::Idle: {
            _createLoopCV.wait_for(continueLoopLock, std::chrono::seconds(1),
                                   [&]() { return _loopState != LoopState::Initializing; });
            break;
        }
        case LoopState::Finishing: {
            return;
        }
        }
    }
}

StreamEventLoop::StreamEventLoop()
{
    _loopThread.reset(new std::thread(&StreamEventLoop::EventLoopThread, this));
    std::unique_lock<std::mutex> lock(_createLoopMutex);
    _createLoopCV.wait(lock, [&]() { return _loopState != LoopState::Initializing; });
}

StreamEventLoop::~StreamEventLoop()
{
    // destroy all Streams?
    // terminate loop
    std::unique_lock<std::mutex> lock(_loopStateMutex);
    _loopState = LoopState::Finishing;
    lock.unlock();
    _continueLoopCV.notify_one();
    _loopThread->join();
}

void StreamEventLoop::scheduleStreamProcess(std::shared_ptr<StreamBase> stream)
{
    std::unique_lock<std::mutex> lockLoopState(_loopStateMutex);
    if (_loopState != LoopState::Finishing)
    {
        std::lock_guard<std::mutex> lockStreamsToProcessQueue(_streamsToProcessQueueMutex);
        _streamsToProcessQueue.push(stream);
        _loopState = LoopState::Working;
    }
    _continueLoopCV.notify_one();
}
