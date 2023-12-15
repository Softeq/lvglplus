#ifndef LVGL_MESSAGE_BOX_PARAMS_HH
#define LVGL_MESSAGE_BOX_PARAMS_HH

#include "widget.hh"
#include "widget_containers/multi_child_widget.hh"
#include "widgets/button.hh"
#include <list>

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for the MessageBox widget
struct MessageBoxParams
{
    static const short defaultWidth;
    static const short defaultHeight;
    static const short defaultButtonHeight;
    static const short defaultBackgroundOpacity;
    static const short defaultBorderWidth;
    static const Padding defaultPadding;
    static const Alignment defaultAlignment;
    struct BoxButton
    {
        std::string text;
        ButtonParamsBuilder buttonParams;
        BoxButton(const std::string &text_, const ButtonParamsBuilder &buttonParams_)
        {
            text = text_;
            buttonParams = buttonParams_;
            if (buttonParams.getParams().height == ButtonParams::defaultHeight)
            {
                buttonParams.height(defaultButtonHeight);
            }
        }
    };
    struct BoxText
    {
        std::string text;
        TextParamsBuilder textParams;
        BoxText(const std::string &text_, const TextParamsBuilder &textParams_)
        {
            text = text_;
            textParams = textParams_;
        }
    };
    BoxText title = BoxText("", TextParamsBuilder());
    BoxText message = BoxText("", TextParamsBuilder());
    std::shared_ptr<std::list<BoxButton>> button = std::make_shared<std::list<BoxButton>>();
    std::function<void()> onCloseCallback = nullptr;
    bool closeButtonEnabled = true;
};

/*
 * The Builder class is used to set the MessageBoxParams for the MessageBox widget that can be passed to the MessageBox
 * constructor.
 * The Builder's setters return a reference to the MessageBoxParamsBuilder object.
 */
template <typename T>
class MessageBoxParamsBuilderBase : public WidgetParamsBuilder<T>
{
protected:
    MessageBoxParams _messageBoxParams;

public:
    const MessageBoxParams &getMessageBoxParams() const
    {
        return _messageBoxParams;
    }

    MessageBoxParamsBuilderBase()
    {
        this->width(MessageBoxParams::defaultWidth);
        this->height(MessageBoxParams::defaultHeight);
        this->alignment(MessageBoxParams::defaultAlignment);
        this->padding(MessageBoxParams::defaultPadding);
        this->backgroundOpacity(MessageBoxParams::defaultBackgroundOpacity);
        this->borderWidth(MessageBoxParams::defaultBorderWidth);
    }

    T &title(const std::string &title_)
    {
        _messageBoxParams.title.text = title_;
        return this->retVal();
    }

    T &title(const std::string &title_, const TextParamsBuilder &textParamsBuilder)
    {
        _messageBoxParams.title.text = title_;
        _messageBoxParams.title.textParams = textParamsBuilder;
        return this->retVal();
    }

    T &text(const std::string &text_)
    {
        _messageBoxParams.message.text = text_;
        return this->retVal();
    }

    T &text(const std::string &text_, const TextParamsBuilder &textParamsBuilder)
    {
        _messageBoxParams.message.text = text_;
        _messageBoxParams.message.textParams = textParamsBuilder;
        return this->retVal();
    }

    T &onClose(const std::function<void()> &onCloseCallback)
    {
        _messageBoxParams.onCloseCallback = onCloseCallback;
        return this->retVal();
    }

    T &addButton(const std::string &text, const ButtonParamsBuilder &buttonParamsBuilder)
    {
        _messageBoxParams.button->push_back(MessageBoxParams::BoxButton(text, buttonParamsBuilder));
        return this->retVal();
    }

    T &addCloseButton(bool enable)
    {
        _messageBoxParams.closeButtonEnabled = enable;
        return this->retVal();
    }
};

class MessageBoxParamsBuilder : public MessageBoxParamsBuilderBase<MessageBoxParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_MESSAGE_BOX_PARAMS_HH
