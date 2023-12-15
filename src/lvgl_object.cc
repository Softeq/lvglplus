#include "lvgl_object.hh"
#include "lvgl_plus.hh"

namespace softeq
{
namespace lvglplus
{

void LvglObject::dispose()
{
    if (_object && (_object != lv_scr_act()))
    {
        UniqueLock lock(LvglPlus::lvglMutex());
        lv_obj_del(_object);
    }
}

LvglObject::~LvglObject()
{
    dispose();
}

lv_obj_t *LvglObject::getObject() const
{
    return _object;
}

void LvglObject::setObject(lv_obj_t *lvObject)
{
    dispose();
    _object = lvObject;
}

void LvglObject::setParent(lv_obj_t *lvObjectParent)
{
    lv_obj_set_parent(_object, lvObjectParent);
}

} // namespace lvglplus
} // namespace softeq