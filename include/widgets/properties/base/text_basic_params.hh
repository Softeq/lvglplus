#ifndef LVGL_PLUS_TEXT_BASIC_PARAMS_HH
#define LVGL_PLUS_TEXT_BASIC_PARAMS_HH

#include "widget.hh"
#include "widgets/properties/alignment.hh"
#include "widgets/properties/base/base_params.hh"
#include <algorithm>
#include <utility>

namespace softeq
{
namespace lvglplus
{
// The structure to hold parameters for widgets with a text
struct TextBasicParams
{
    static const int minFontSize;
    static const int maxFontSize;
};

static lv_font_t *convertTextSizeToFont(int fontSize)
{
    // if the font size is out of range set it to the closest border
    fontSize = std::min(TextBasicParams::maxFontSize, std::max(TextBasicParams::minFontSize, fontSize));
    const lv_font_t *fontSizes[] = {&lv_font_montserrat_8,  &lv_font_montserrat_10, &lv_font_montserrat_12,
                                    &lv_font_montserrat_14, &lv_font_montserrat_16, &lv_font_montserrat_18,
                                    &lv_font_montserrat_20, &lv_font_montserrat_22, &lv_font_montserrat_24,
                                    &lv_font_montserrat_26, &lv_font_montserrat_28, &lv_font_montserrat_30,
                                    &lv_font_montserrat_32, &lv_font_montserrat_34, &lv_font_montserrat_36,
                                    &lv_font_montserrat_38, &lv_font_montserrat_40};
    int fontSizeIndex = fontSize / 2 - 4;

    const int fontsQuantity = sizeof(fontSizes) / sizeof(fontSizes[0]);
    fontSizeIndex = std::min(fontsQuantity - 1, std::max(0, fontSizeIndex));
    return const_cast<lv_font_t *>(fontSizes[fontSizeIndex]);
}

/*
 * The Builder class is used to set the TextBasicParams for the widget with a text that can be passed as a part of the
 * base Builder to the widget constructor.
 * The Builder's setters return a reference to the templated (derived) Builder object.
 */
template <typename T>
class TextBasicParamsBuilder : virtual public BaseParamsBuilder<T>
{
public:
    TextBasicParamsBuilder() = default;

    T &fontColor(long color)
    {
        this->addInstruction([color](lv_obj_t *obj) { lv_obj_set_style_text_color(obj, lv_color_hex(color), 0); });
        return this->retVal();
    }

    T &fontSize(int fontSize)
    {
        this->addInstruction(
            [fontSize](lv_obj_t *obj) { lv_obj_set_style_text_font(obj, convertTextSizeToFont(fontSize), 0); });
        return this->retVal();
    }

    T &fontAlign(TextAlignment alignment)
    {
        this->addInstruction(
            [alignment](lv_obj_t *obj) { lv_obj_set_style_text_align(obj, static_cast<uint8_t>(alignment), 0); });
        return this->retVal();
    }
};

class TextOwner
{
private:
    lv_obj_t *_lvObject;
    static const unsigned int mainPart = 0;

protected:
    explicit TextOwner(lv_obj_t *lvObject)
    {
        _lvObject = lvObject;
    }

public:
    void setFontColor(long color)
    {
        lv_obj_set_style_text_color(_lvObject, lv_color_hex(color), mainPart);
    }

    void setFontSize(int size)
    {
        lv_obj_set_style_text_font(_lvObject, convertTextSizeToFont(size), mainPart);
    }

    void setFontAlign(TextAlignment alignment)
    {
        lv_obj_set_style_text_align(_lvObject, static_cast<uint8_t>(alignment), mainPart);
    }
};

} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_TEXT_BASIC_PARAMS_HH
