#ifndef LVGL_PLUS_SDL_DRV_HH
#define LVGL_PLUS_SDL_DRV_HH

#include "lv_drv_conf.h"

#if USE_SDL

#include "lvgl.h"
#include "lvgl_plus.hh"

/**
 * Initialize SDL to be used as display, mouse and mouse wheel drivers.
 */
void sdlInit(void);

/**
 * Add SDL display first.
 * @param disp pointer to LVGL display driver
 */
void sdlAddDisplay(lv_disp_drv_t *disp);

/**
 * Add SDL input drivers to display. If there are several mouses/keyboards, specify device id.
 * @param disp pointer to LVGL display driver
 * @param id of the device
 */
void sdlSetMouse(lv_disp_drv_t *disp, uint32_t id = 0);
void sdlSetWheel(lv_disp_drv_t *disp, uint32_t id = 0);
void sdlSetKeyboard(lv_disp_drv_t *disp, uint32_t id = 0);

/**
 * Flush a buffer to the marked area
 * @param drv pointer to driver where this function belongs
 * @param area an area where to copy `color_p`
 * @param color_p an array of pixel to copy to the `area` part of the screen
 */
void sdlDisplayFlush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);

/**
 * Get the current position and state of the mouse
 * @param indev_drv pointer to the related input device driver
 * @param data store the mouse data here
 */
void sdlMouseRead(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

/**
 * Get encoder (i.e. mouse wheel) ticks difference and pressed state
 * @param indev_drv pointer to the related input device driver
 * @param data store the read data here
 */
void sdlMousewheelRead(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

/**
 * Get input from the keyboard.
 * @param indev_drv pointer to the related input device driver
 * @param data store the red data here
 */
void sdlKeyboardRead(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);

/**
 * Initialize SDL, add display and all needed drivers.
 * @param displayWidth width (horisontal resolution) of the SDL display
 * @param displayHeight height (vertical resolution) of the SDL display
 * @param addMouse initialize SDL mouse and add to LVGL
 * @param addWheel initialize SDL wheel and add to LVGL
 * @param addKeyboard initialize SDL keyboard and add to LVGL
 */
void addSdlPeripherals(unsigned short displayWidth, unsigned short displayHeight, bool addMouse = true,
                       bool addWheel = true, bool addKeyboard = true);

#endif /* USE_SDL */

#endif //LVGL_PLUS_SDL_DRV_HH
