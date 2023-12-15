#ifndef LVGL_PLUS_WIDGET_PARAMS_H
#define LVGL_PLUS_WIDGET_PARAMS_H
#include "widgets/properties/alignment.hh"
#include "widgets/properties/base/base_params.hh"
#include "widgets/properties/padding.hh"
#include "widgets/properties/point.hh"
#include <list>
#include <memory>
#include <src/misc/lv_area.h>

namespace softeq
{
namespace lvglplus
{
using Offset = Point;

// The structure to hold parameters for the LinearProgressIndicator widget
struct WidgetParams
{
    short width = LV_PCT(100);
    short height = LV_PCT(100);
    int zIndex = 0;
    std::shared_ptr<std::list<std::function<void(lv_obj_t *obj)>>> instructions;
};

/*
 * The Builder class is used to set the WidgetParams for the base widget that can be passed to the Widget constructor.
 * The Builder's setters return a reference to the templated (derived) Builder object.
 */
template <typename T>
class WidgetParamsBuilder : virtual public BaseParamsBuilder<T>
{
protected:
    WidgetParams _widgetParams;

public:
    const WidgetParams &getParams() const
    {
        return _widgetParams;
    }

    WidgetParamsBuilder()
        : BaseParamsBuilder<T>()
    {
        _widgetParams.instructions = this->instructions();
    }

    T &width(short width)
    {
        this->addInstruction([width](lv_obj_t *obj) { lv_obj_set_width(obj, width); });
        return this->retVal();
    }

    T &widthEqContent()
    {
        this->addInstruction([](lv_obj_t *obj) { lv_obj_set_width(obj, LV_SIZE_CONTENT); });
        return this->retVal();
    }

    T &widthPctOfParent(short value)
    {
        this->addInstruction([value](lv_obj_t *obj) { lv_obj_set_width(obj, LV_PCT(value)); });
        return this->retVal();
    }

    T &height(short height)
    {
        this->addInstruction([height](lv_obj_t *obj) { lv_obj_set_height(obj, height); });
        return this->retVal();
    }

    T &heightEqContent()
    {
        this->addInstruction([](lv_obj_t *obj) { lv_obj_set_height(obj, LV_SIZE_CONTENT); });
        return this->retVal();
    }

    T &heightPctOfParent(short value)
    {
        this->addInstruction([value](lv_obj_t *obj) { lv_obj_set_height(obj, LV_PCT(value)); });
        return this->retVal();
    }

    T &size(short width, short height)
    {
        this->addInstruction([width](lv_obj_t *obj) { lv_obj_set_width(obj, width); });
        this->addInstruction([height](lv_obj_t *obj) { lv_obj_set_height(obj, height); });
        return this->retVal();
    }

    T &sizeEqContent()
    {
        this->addInstruction([](lv_obj_t *obj) { lv_obj_set_width(obj, LV_SIZE_CONTENT); });
        this->addInstruction([](lv_obj_t *obj) { lv_obj_set_height(obj, LV_SIZE_CONTENT); });
        return this->retVal();
    }

    T &borderWidth(short borderWidth)
    {
        this->addInstruction([borderWidth](lv_obj_t *obj) { lv_obj_set_style_border_width(obj, borderWidth, 0); });
        return this->retVal();
    }

    T &borderColor(long color)
    {
        this->addInstruction([color](lv_obj_t *obj) { lv_obj_set_style_border_color(obj, lv_color_hex(color), 0); });
        return this->retVal();
    }

    T &alignment(const Alignment &alignment)
    {
        this->addInstruction([alignment](lv_obj_t *obj) { lv_obj_set_align(obj, static_cast<uint8_t>(alignment)); });
        return this->retVal();
    }

    T &padding(const Padding &padding)
    {
        this->addInstruction([padding](lv_obj_t *obj) {
            lv_obj_set_style_pad_top(obj, padding.getTop(), 0);
            lv_obj_set_style_pad_bottom(obj, padding.getBottom(), 0);
            lv_obj_set_style_pad_left(obj, padding.getLeft(), 0);
            lv_obj_set_style_pad_right(obj, padding.getRight(), 0);
            lv_obj_set_style_pad_row(obj, padding.getRow(), 0);
            lv_obj_set_style_pad_column(obj, padding.getColumn(), 0);
        });
        return this->retVal();
    }

    T &offset(const Offset &offset_)
    {
        return offset(offset_.x, offset_.y);
    }

    T &offset(short x, short y)
    {
        this->addInstruction([x, y](lv_obj_t *obj) {
            lv_obj_set_x(obj, x);
            lv_obj_set_y(obj, y);
        });
        return this->retVal();
    }

    T &opacity(const unsigned short &opacity)
    {
        this->addInstruction([opacity](lv_obj_t *obj) { lv_obj_set_style_opa(obj, opacity, LV_PART_MAIN); });
        return this->retVal();
    }

    T &backgroundColor(const unsigned long &color)
    {
        this->addInstruction([color](lv_obj_t *obj) {
            if ((lv_obj_get_style_bg_opa(obj, 0) == LV_OPA_TRANSP) || (lv_obj_get_style_bg_opa(obj, 0) == LV_OPA_COVER))
            {
                lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, LV_PART_MAIN);
            }
            lv_obj_set_style_bg_color(obj, lv_color_hex(color), LV_PART_MAIN);
            lv_obj_set_style_bg_grad_dir(obj, LV_GRAD_DIR_NONE, LV_PART_MAIN);
        });
        return this->retVal();
    }

    T &backgroundOpacity(const unsigned short &opacity)
    {
        this->addInstruction([opacity](lv_obj_t *obj) { lv_obj_set_style_bg_opa(obj, opacity, LV_PART_MAIN); });
        return this->retVal();
    }

    /**
     * @brief Sets a gradient color to the background.
     *
     * @param vertical A vertical (1) gradient direction if true. Horizontal (2) - otherwise.
     * @param color1 The color at the top(1)/left(2).
     * @param color2 The color at the bottom(1)/right(2).
     * @return The parameter builder.
     */
    T &backgroundGradient(long color1, long color2, bool vertical = true)
    {
        backgroundColor(color1);
        lv_grad_dir_t dir;
        if (vertical)
        {
            dir = LV_GRAD_DIR_VER;
        }
        else
        {
            dir = LV_GRAD_DIR_HOR;
        }
        this->addInstruction([color2, dir](lv_obj_t *obj) {
            lv_obj_set_style_bg_grad_dir(obj, dir, LV_PART_MAIN);
            lv_obj_set_style_bg_grad_color(obj, lv_color_hex(color2), LV_PART_MAIN);
        });
        return this->retVal();
    }

    T &radius(short value)
    {
        this->addInstruction([value](lv_obj_t *obj) { lv_obj_set_style_radius(obj, value, LV_PART_MAIN); });
        return this->retVal();
    }

    T &zIndex(short value)
    {
        _widgetParams.zIndex = value;
        return this->retVal();
    }
};

} // namespace lvglplus
} // namespace softeq
#endif // LVGL_PLUS_WIDGET_PARAMS_H
