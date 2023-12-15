#ifndef LVGL_PLUS_LVGL_DISPLAY_HH
#define LVGL_PLUS_LVGL_DISPLAY_HH
#include "lvgl.h"
#include <functional>
#include <iostream>
#include <memory>
namespace softeq
{
namespace lvglplus
{

class LvglDisplay
{
public:
    LvglDisplay(unsigned short width, unsigned short height,
                void (*flushCallback)(struct _lv_disp_drv_t *dispDrv, const lv_area_t *area, lv_color_t *colorArray))
    {
        createDisplay(width, height, flushCallback);
    }

    ~LvglDisplay()
    {
        lv_disp_remove(_disp);
    }

    lv_disp_t *lvDisplay()
    {
        return _disp;
    }

    void setAsDefault()
    {
        lv_disp_set_default(_disp);
    }

private:
    void createDisplay(unsigned short width, unsigned short height,
                       void (*flushCallback)(struct _lv_disp_drv_t *dispDrv, const lv_area_t *area,
                                             lv_color_t *colorArray))
    {
        /*Create a display buffer*/
        uint32_t sizeInPixels = width * height;
        buffer = std::unique_ptr<lv_color_t[]>(new lv_color_t[sizeInPixels]);
        lv_disp_draw_buf_init(&lvglDisplayBuffer, buffer.get(), nullptr, sizeInPixels);

        /*Create a display*/
        lv_disp_drv_init(&disp_drv); /*Basic initialization*/
        disp_drv.draw_buf = &lvglDisplayBuffer;
        disp_drv.flush_cb = flushCallback;
        disp_drv.hor_res = width;
        disp_drv.ver_res = height;
        disp_drv.user_data = this;
        _disp = lv_disp_drv_register(&disp_drv);
    }

    lv_disp_t *_disp;
    std::unique_ptr<lv_color_t[]> buffer;
    lv_disp_drv_t disp_drv;
    lv_disp_draw_buf_t lvglDisplayBuffer;
};

} // namespace lvglplus
} // namespace softeq
#endif //LVGL_PLUS_LVGL_DISPLAY_HH
