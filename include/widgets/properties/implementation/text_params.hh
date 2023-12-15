#ifndef LVGL_TEXT_PARAMS_HH
#define LVGL_TEXT_PARAMS_HH

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
struct TextParams : public TextBasicParams
{
    static const short defaultWidth;
    static const short defaultHeight;
};

/*
 * The Builder class is used to set the TextParams for the Text widget that can be passed to the Text constructor.
 * The Builder's setters return a reference to the TextParamsBuilderBase object.
 */
template <typename T>
class TextParamsBuilderBase : public WidgetParamsBuilder<T>, public TextBasicParamsBuilder<T>
{
protected:
    TextParams _textParams;

public:
    const TextParams &getTextParams() const
    {
        return _textParams;
    }

    TextParamsBuilderBase()
        : WidgetParamsBuilder<T>()
        , TextBasicParamsBuilder<T>()
    {
        this->width(TextParams::defaultWidth);
        this->height(TextParams::defaultHeight);
    }

    explicit TextParamsBuilderBase(const TextBasicParams &textParams)
        : TextParamsBuilderBase()
    {
        (TextBasicParams &)_textParams = textParams;
    }
};

class TextParamsBuilder : public TextParamsBuilderBase<TextParamsBuilder>
{
public:
    TextParamsBuilder(const TextBasicParams &textParams)
        : TextParamsBuilderBase<TextParamsBuilder>(textParams)
    {
    }
    TextParamsBuilder()
        : TextParamsBuilderBase<TextParamsBuilder>()
    {
    }
};
} // namespace lvglplus
} // namespace softeq

#endif // LVGL_TEXT_PARAMS_HH
