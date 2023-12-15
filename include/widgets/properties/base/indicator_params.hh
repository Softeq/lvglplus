#ifndef LVGL_PLUS_INDICATOR_PARAMS_HH
#define LVGL_PLUS_INDICATOR_PARAMS_HH

#include "lvgl.h"
#include "lvgl_plus.hh"
#include "widgets/properties/base/base_params.hh"

namespace softeq
{
namespace lvglplus
{

/*
 * The Builder class is used to set the IndicatorParams for the widget with an indicator that can be passed as a part
 * of the base Builder to the widget constructor.
 * The Builder's setters return a reference to the templated (derived) Builder object.
 */
template <typename T>
class IndicatorParamsBuilder : virtual public BaseParamsBuilder<T>
{
public:
    IndicatorParamsBuilder() = default;

    T &indicatorColor(long color)
    {
        this->addInstruction(
            [color](lv_obj_t *obj) { lv_obj_set_style_bg_color(obj, lv_color_hex(color), LV_PART_INDICATOR); });
        return this->retVal();
    }

    T &indicatorBorderWidth(short value)
    {
        this->addInstruction([value](lv_obj_t *obj) { lv_obj_set_style_border_width(obj, value, LV_PART_INDICATOR); });
        return this->retVal();
    }

    T &indicatorBorderColor(long color)
    {
        this->addInstruction(
            [color](lv_obj_t *obj) { lv_obj_set_style_border_color(obj, lv_color_hex(color), LV_PART_INDICATOR); });
        return this->retVal();
    }
};

class IndicatorOwner
{
public:
    virtual short indicatorBorderWidth()
    {
        return static_cast<short>(lv_obj_get_style_border_width(_lvObject, LV_PART_INDICATOR));
    }

    virtual void setIndicatorBorderWidth(short value)
    {
        UniqueLock locker(LvglPlus::lvglMutex());
        lv_obj_set_style_border_width(_lvObject, value, LV_PART_INDICATOR);
    }

    virtual long indicatorBorderColor()
    {
        return static_cast<long>(lv_obj_get_style_border_color(_lvObject, LV_PART_INDICATOR).full & 0xffffff);
    }

    virtual void setIndicatorBorderColor(long color)
    {
        UniqueLock locker(LvglPlus::lvglMutex());
        lv_obj_set_style_border_color(_lvObject, lv_color_hex(color), LV_PART_INDICATOR);
    }

    virtual long indicatorColor()
    {
        return static_cast<long>(lv_obj_get_style_bg_color(_lvObject, LV_PART_INDICATOR).full & 0xffffff);
    }

    virtual void setIndicatorColor(long color)
    {
        UniqueLock locker(LvglPlus::lvglMutex());
        lv_obj_set_style_bg_color(_lvObject, lv_color_hex(color), LV_PART_INDICATOR);
    }

protected:
    lv_obj_t *_lvObject = nullptr;

    explicit IndicatorOwner(lv_obj_t *lvObject)
    {
        _lvObject = lvObject;
    }
};

} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_INDICATOR_PARAMS_HH
