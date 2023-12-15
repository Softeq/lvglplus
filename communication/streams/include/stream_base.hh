#ifndef LVGL_PLUS_STREAM_BASE_H
#define LVGL_PLUS_STREAM_BASE_H
#include <functional>

namespace softeq
{
namespace streams
{

using VoidCallback = std::function<void()>;
class StreamBase
{
public:
    virtual bool notifyListener() = 0;
    virtual ~StreamBase() = default;
    virtual void cancel() = 0;
    virtual bool hasListener() = 0;
    std::function<void(void)> notifyController;
};
} // namespace streams
} // namespace softeq

#endif // LVGL_PLUS_STREAM_BASE_H
