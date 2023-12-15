#ifndef TEST_STREAM_SINK_H
#define TEST_STREAM_SINK_H
#include <communication/streams/include/event_sink.hh>
#include <communication/streams/include/stream_consumer.hh>

namespace softeq
{
namespace streams
{

/// A object that accepts stream events both synchronously and asynchronously.
///
/// A [StreamSink] combines the methods from [StreamConsumer] and [EventSink].
///
template <typename T>
class StreamSink : public EventSink<T>, public StreamConsumer<T>
{
    /// Tells the stream sink that no further _streams will be added.
    //virtual void close() = 0;
    /// Return a future which is completed when the [StreamSink] is finished.
    //virtual void done() = 0;
};
} // namespace streams
} // namespace softeq

#endif // TEST_STREAM_SINK_H
