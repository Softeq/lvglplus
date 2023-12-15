#ifndef LVGL_PLUS_LED_HH
#define LVGL_PLUS_LED_HH

#include "lvgl.h"
#include "lvgl_plus.hh"
#include "properties/implementation/led_params.hh"
#include "widget.hh"
#include <utility>

namespace softeq
{
namespace lvglplus
{

class Led : public Widget
{
public:
    Led();
    explicit Led(const LedParamsBuilder &params);
    ~Led() override;

    unsigned char brightness()
    {
        return static_cast<unsigned char>(lv_led_get_brightness(lvglObject.getObject()));
    }
    void setBrightness(unsigned char value)
    {
        UniqueLock locker(LvglPlus::lvglMutex());
        lv_led_set_brightness(lvglObject.getObject(), value);
    }
    void setState(bool state)
    {
        if (state)
        {
            on();
        }
        else
        {
            off();
        }
    }
    void on()
    {
        UniqueLock locker(LvglPlus::lvglMutex());
        lv_led_on(lvglObject.getObject());
    }
    void off()
    {
        UniqueLock locker(LvglPlus::lvglMutex());
        lv_led_off(lvglObject.getObject());
    }
    void toggle()
    {
        UniqueLock locker(LvglPlus::lvglMutex());
        lv_led_toggle(lvglObject.getObject());
    }
    void setColor(long color)
    {
        UniqueLock locker(LvglPlus::lvglMutex());
        lv_led_set_color(lvglObject.getObject(), lv_color_hex(color));
    }

private:
    void render() override;
};

} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_LED_HH
