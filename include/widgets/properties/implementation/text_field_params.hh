#ifndef LVGL_TEXT_FIELD_PARAMS_HH
#define LVGL_TEXT_FIELD_PARAMS_HH

#include "controllers/text_editing_controller.hh"
#include "text_controllable.hh"
#include "widget.hh"
#include "widget_containers/single_child_widget.hh"
#include "widgets/properties/base/text_basic_params.hh"

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for the LinearProgressIndicator widget
struct TextFieldParams : public TextBasicParams
{
    static const short defaultWidth;
    static const short defaultHeight;
    static const short defaultPadding;
};

/*
 * The Builder class is used to set the TextFieldParams for the TextField widget that can be passed to the TextField
 * constructor.
 * The Builder's setters return a reference to the TextFieldParamsBuilder object.
 */
template <typename T>
class TextFieldParamsBuilderBase : public WidgetParamsBuilder<T>, public TextBasicParamsBuilder<T>
{
protected:
    TextFieldParams _textFieldParams;

public:
    const TextFieldParams &getTextFieldParams() const
    {
        return _textFieldParams;
    }

    TextFieldParamsBuilderBase()
        : WidgetParamsBuilder<T>()
        , TextBasicParamsBuilder<T>()
    {
        this->width(TextFieldParams::defaultWidth);
        this->height(TextFieldParams::defaultHeight);
        this->padding(Padding().top(TextFieldParams::defaultPadding).left(TextFieldParams::defaultPadding));
    }
};

class TextFieldParamsBuilder : public TextFieldParamsBuilderBase<TextFieldParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_TEXT_FIELD_PARAMS_HH
