#ifndef LVGL_PLUS_KNOB_PARAMS_HH
#define LVGL_PLUS_KNOB_PARAMS_HH

#include "lvgl.h"
#include "widget.hh"
#include "widgets/properties/base/base_params.hh"

namespace softeq
{
namespace lvglplus
{
/*
 * The Builder class is used to set the KnobParams for the widget with a knob that can be passed as a part of the base
 * Builder to the widget constructor.
 * The Builder's setters return a reference to the templated (derived) Builder object.
 */
template <typename T>
class KnobParamsBuilder : virtual public BaseParamsBuilder<T>
{
public:
    KnobParamsBuilder() = default;

    T &knobColor(long color)
    {
        this->addInstruction(
            [color](lv_obj_t *obj) { lv_obj_set_style_bg_color(obj, lv_color_hex(color), LV_PART_INDICATOR); });
        return this->retVal();
    }

    T &knobBorderWidth(short value)
    {
        this->addInstruction([value](lv_obj_t *obj) { lv_obj_set_style_border_width(obj, value, LV_PART_KNOB); });
        return this->retVal();
    }

    T &knobBorderColor(long color)
    {
        this->addInstruction(
            [color](lv_obj_t *obj) { lv_obj_set_style_border_color(obj, lv_color_hex(color), LV_PART_INDICATOR); });
        return this->retVal();
    }

    T &knobSize(short value)
    {
        this->addInstruction([value](lv_obj_t *obj) { lv_obj_set_style_pad_all(obj, value, LV_PART_KNOB); });
        return this->retVal();
    }
};

class KnobOwner
{
public:
    virtual short knobBorderWidth()
    {
        return static_cast<short>(lv_obj_get_style_border_width(_lvObject, LV_PART_KNOB));
    }

    virtual void setKnobBorderWidth(short value)
    {
        lv_obj_set_style_border_width(_lvObject, value, LV_PART_KNOB);
    }

    virtual long knobBorderColor()
    {
        return static_cast<long>(lv_obj_get_style_border_color(_lvObject, LV_PART_KNOB).full & 0xffffff);
    }

    virtual void setKnobBorderColor(long color)
    {
        lv_obj_set_style_border_color(_lvObject, lv_color_hex(color), LV_PART_KNOB);
    }

    virtual long knobColor()
    {
        return static_cast<long>(lv_obj_get_style_bg_color(_lvObject, LV_PART_KNOB).full & 0xffffff);
    }

    virtual void setKnobColor(long color)
    {
        lv_obj_set_style_bg_color(_lvObject, lv_color_hex(color), LV_PART_KNOB);
    }

    virtual short knobSize()
    {
        return static_cast<short>(lv_obj_get_style_pad_left(_lvObject, LV_PART_KNOB));
    }

    virtual void setKnobSize(short value)
    {
        lv_obj_set_style_pad_all(_lvObject, value, LV_PART_KNOB);
    }

protected:
    lv_obj_t *_lvObject = nullptr;

    explicit KnobOwner(lv_obj_t *lvObject)
    {
        _lvObject = lvObject;
    }
};

} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_KNOB_PARAMS_HH
