#ifndef LVGL_PLUS_CHECKBOX_PARAMS_HH
#define LVGL_PLUS_CHECKBOX_PARAMS_HH

#include "widget.hh"
#include "widgets/properties/base/indicator_params.hh"
#include "widgets/properties/base/text_basic_params.hh"
#include <utility>

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for the Checkbox widget
struct CheckboxParams : public TextBasicParams
{
    static const short defaultWidth;
    static const short defaultHeight;
    std::string text;
    std::function<void(void)> onChangeCallback = nullptr;
};

/*
 * The Builder class is used to set the CheckboxParams for the Checkbox widget that can be passed to the Checkbox
 * constructor.
 * The Builder's setters return a reference to the CheckboxParamsBuilder object.
 */
template <typename T>
class CheckboxParamsBuilderBase : public WidgetParamsBuilder<T>,
                                  public TextBasicParamsBuilder<T>,
                                  public IndicatorParamsBuilder<T>
{
protected:
    CheckboxParams _checkboxParams;

public:
    const CheckboxParams &getCheckboxParams() const
    {
        return _checkboxParams;
    }

    CheckboxParamsBuilderBase()
        : WidgetParamsBuilder<T>()
        , TextBasicParamsBuilder<T>()
        , IndicatorParamsBuilder<T>()
    {
        this->width(CheckboxParams::defaultWidth);
        this->height(CheckboxParams::defaultHeight);
    }

    const T &text(const std::string &newText)
    {
        _checkboxParams.text = newText;
        return this->retVal();
    }

    const T &setOnChangeCallback(std::function<void(void)> callback)
    {
        _checkboxParams.onChangeCallback = std::move(callback);
        return this->retVal();
    }

    const T &inactiveColor(long color)
    {
        this->addInstruction(
            [color](lv_obj_t *obj) { lv_obj_set_style_bg_color(obj, lv_color_hex(color), LV_PART_INDICATOR); });
        return this->retVal();
    }
};

class CheckboxParamsBuilder : public CheckboxParamsBuilderBase<CheckboxParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_CHECKBOX_PARAMS_HH
