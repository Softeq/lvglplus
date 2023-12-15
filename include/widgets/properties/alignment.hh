#ifndef LVGL_ALIGNMENT_HH
#define LVGL_ALIGNMENT_HH
#include "lvgl.h"
namespace softeq
{
namespace lvglplus
{

enum class Alignment
{
    bottomCenter = LV_ALIGN_BOTTOM_MID,
    bottomLeft = LV_ALIGN_BOTTOM_LEFT,
    bottomRight = LV_ALIGN_BOTTOM_RIGHT,
    center = LV_ALIGN_CENTER,
    centerLeft = LV_ALIGN_LEFT_MID,
    centerRight = LV_ALIGN_RIGHT_MID,
    topCenter = LV_ALIGN_TOP_MID,
    topLeft = LV_ALIGN_TOP_LEFT,
    topRight = LV_ALIGN_TOP_RIGHT,
    defaultAlignment = LV_ALIGN_DEFAULT,
};

enum class TextAlignment
{
    center = LV_TEXT_ALIGN_CENTER,
    left = LV_TEXT_ALIGN_LEFT,
    right = LV_TEXT_ALIGN_RIGHT,
};

enum class ChildrenAlignment
{
    start = LV_FLEX_ALIGN_START,
    end = LV_FLEX_ALIGN_END,
    center = LV_FLEX_ALIGN_CENTER,
    spaceEvenly = LV_FLEX_ALIGN_SPACE_EVENLY,
    spaceAround = LV_FLEX_ALIGN_SPACE_AROUND,
    spaceBetween = LV_FLEX_ALIGN_SPACE_BETWEEN,
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_ALIGNMENT_HH
