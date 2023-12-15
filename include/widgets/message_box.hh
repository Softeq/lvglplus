#ifndef LVGL_MESSAGE_BOX_HH
#define LVGL_MESSAGE_BOX_HH

#include "lvgl.h"
#include "lvgl_display.hh"
#include "properties/implementation/message_box_params.hh"
#include "widget.hh"

namespace softeq
{
namespace lvglplus
{

class MessageBox : public Widget
{
public:
    MessageBox(const std::shared_ptr<LvglDisplay> &display);
    explicit MessageBox(const std::shared_ptr<LvglDisplay> &display, const MessageBoxParamsBuilder &params);
    ~MessageBox() override = default;
    void setOnExit(std::function<void()> onExitCallback)
    {
        _onExitCallback = onExitCallback;
    }

private:
    std::function<void()> _onExitCallback;
    std::function<void()> _onCloseCallback;
    void render() override;
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_MESSAGE_BOX_HH
