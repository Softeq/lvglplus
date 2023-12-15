#ifndef LVGL_PLUS_LVGLOBJECT_HH
#define LVGL_PLUS_LVGLOBJECT_HH

#include "lvgl.h"
#include <functional>
#include <iostream>

namespace softeq
{
namespace lvglplus
{

class LvglObject
{
public:
    ~LvglObject();
    lv_obj_t *getObject() const;
    void setObject(lv_obj_t *lvObject);
    void setParent(lv_obj_t *lvObjectParent);
    void dispose();

private:
    /// Native lvgl widget pointer
    lv_obj_t *_object = nullptr;
};

} // namespace lvglplus
} // namespace softeq
#endif // LVGL_PLUS_LVGLOBJECT_HH
