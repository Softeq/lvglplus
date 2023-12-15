#ifndef LVGL_PLUS_LVGL_DRIVER_HH
#define LVGL_PLUS_LVGL_DRIVER_HH
#include "lvgl.h"
namespace softeq
{
namespace lvglplus
{

class LvglDriver
{
public:
    LvglDriver(lv_indev_type_t type, void (*readCallback)(struct _lv_indev_drv_t *indev_drv, lv_indev_data_t *data))
    {
        createDriver(type, readCallback);
    }
    ~LvglDriver()
    {
        lv_indev_delete(_indev);
    }
    lv_indev_t *indev()
    {
        return _indev;
    }

private:
    void createDriver(lv_indev_type_t type,
                      void (*readCallback)(struct _lv_indev_drv_t *indev_drv, lv_indev_data_t *data))
    {
        _lvGroup = lv_group_create();
        lv_group_set_default(_lvGroup);
        lv_indev_drv_init(&_indevDrv); /*Basic initialization*/
        _indevDrv.type = type;
        _indevDrv.read_cb = readCallback;
        _indev = lv_indev_drv_register(&_indevDrv);
        lv_indev_set_group(_indev, _lvGroup);
    }
    lv_group_t *_lvGroup;
    lv_indev_drv_t _indevDrv;
    lv_indev_t *_indev;
};

} // namespace lvglplus
} // namespace softeq
#endif //LVGL_PLUS_LVGL_DRIVER_HH
