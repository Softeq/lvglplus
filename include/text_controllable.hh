#ifndef LVGL_PLUS_TEXT_CONTROLLABLE_HH
#define LVGL_PLUS_TEXT_CONTROLLABLE_HH
#include "controllable.hh"
#include "controllers/text_controller.hh"
#include <memory>
#include <string>

namespace softeq
{
namespace lvglplus
{
class TextController;

class TextControllable : public Controllable
{
public:
    virtual void setText(std::string) = 0;
    virtual std::string getText() = 0;

protected:
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_PLUS_TEXT_CONTROLLABLE_HH
