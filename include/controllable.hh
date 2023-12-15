#ifndef LVGL_PLUS_CONTROLLABLE_HH
#define LVGL_PLUS_CONTROLLABLE_HH
#include "controllers/text_controller.hh"
#include <memory>
#include <string>

namespace softeq
{
namespace lvglplus
{
class Controller;

class Controllable
{
public:
    virtual void setController(const std::shared_ptr<Controller> &) = 0;
    virtual std::shared_ptr<Controller> getController() = 0;

protected:
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_PLUS_CONTROLLABLE_HH
