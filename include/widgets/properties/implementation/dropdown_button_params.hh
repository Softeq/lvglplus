#ifndef LVGL_PLUS_DROPDOWN_BUTTON_PARAMS_HH
#define LVGL_PLUS_DROPDOWN_BUTTON_PARAMS_HH

#include "widget.hh"
#include "widgets/properties/base/indicator_params.hh"
#include "widgets/properties/base/text_basic_params.hh"
#include <utility>

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for the DropDownButton widget
struct DropDownButtonParams : public TextBasicParams
{
    static const short defaultWidth;
    static const short defaultHeight;
    std::function<void(void)> onChangeCallback = nullptr;
};

/*
 * The Builder class is used to set the DropDownButtonParams for the DropDownButton widget that can be passed to the
 * DropDownButton constructor.
 * The Builder's setters return a reference to the DropDownButtonParamsBuilder object.
 */
template <typename T>
class DropDownButtonParamsBuilderBase : public WidgetParamsBuilder<T>,
                                        public TextBasicParamsBuilder<T>,
                                        public IndicatorParamsBuilder<T>
{
protected:
    DropDownButtonParams _dropDownButtonParams;

public:
    const DropDownButtonParams &getDropDownButtonParams() const
    {
        return _dropDownButtonParams;
    }

    DropDownButtonParamsBuilderBase()
        : WidgetParamsBuilder<T>()
        , TextBasicParamsBuilder<T>()
        , IndicatorParamsBuilder<T>()
    {
        this->width(DropDownButtonParams::defaultWidth);
        this->height(DropDownButtonParams::defaultHeight);
    }

    T &onChange(std::function<void(void)> handler)
    {
        _dropDownButtonParams.onChangeCallback = std::move(handler);
        return this->retVal();
    }
};

class DropDownButtonParamsBuilder : public DropDownButtonParamsBuilderBase<DropDownButtonParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_DROPDOWN_BUTTON_PARAMS_HH
