#ifndef TEST_EVENT_SINK_H
#define TEST_EVENT_SINK_H

namespace softeq
{
namespace streams
{

/// A [Sink] that supports adding errors.
///
/// This makes it suitable for capturing the results of asynchronous
/// computations, which can complete with a value or an error.
template <typename T>
class EventSink
{
    /// Adds a data [event] to the sink.
    virtual void add(T event) = 0;

    /// Adds an [error] to the sink.
    // void addError(Object error, [StackTrace? stackTrace]);

    /// Closes the sink.
    // virtual void close() = 0;
};
} // namespace streams
} // namespace softeq
#endif // TEST_EVENT_SINK_H
