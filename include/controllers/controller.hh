#ifndef LVGL_PLUS_CONTROLLER_HH
#define LVGL_PLUS_CONTROLLER_HH

#include "controllable.hh"
#include <memory>

namespace softeq
{
namespace lvglplus
{

class Controller
{
public:
    virtual ~Controller() = default;

    void setWidget(std::weak_ptr<Controllable> widget)
    {
        userWidget = std::move(widget);
    }

protected:
    std::weak_ptr<Controllable> userWidget;
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_PLUS_CONTROLLER_HH
