#ifndef TEST_STREAM_CONSUMER_H
#define TEST_STREAM_CONSUMER_H
#include <communication/streams/include/stream.hh>

namespace softeq
{
namespace streams
{

/// Abstract interface for a "sink" accepting multiple entire _streams.
///
/// A consumer can accept a number of consecutive _streams using [addStream],
/// and when no further data need to be added, the [close] method tells the
/// consumer to complete its work and shut down.
template <typename S>
class StreamConsumer
{
    //virtual void addStream(Stream<S> stream) = 0;
    //virtual void close() = 0;
};
} // namespace streams
} // namespace softeq

#endif // TEST_STREAM_CONSUMER_H
