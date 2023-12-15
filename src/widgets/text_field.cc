#include "widgets/text_field.hh"
#include "lvgl_plus.hh"

namespace softeq
{
namespace lvglplus
{
const short TextFieldParams::defaultWidth = 100;
const short TextFieldParams::defaultHeight = 30;
const short TextFieldParams::defaultPadding = 5;

TextField::TextField(std::string text, const TextFieldParamsBuilder &params)
    : Widget("TextField", params.getParams(), lv_textarea_create)
    , TextOwner(this->lvglObject.getObject())
{
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::noChild);
    _text = std::move(text);
    UniqueLock locker(LvglPlus::lvglMutex());
    auto object = lvglObject.getObject();
    if (width() == LV_SIZE_CONTENT)
    {
        setWidth(TextFieldParams::defaultWidth);
    }
    lv_obj_add_event_cb(object, &TextField::onTextChanged, LV_EVENT_VALUE_CHANGED, this);
    lv_textarea_set_text(object, _text.c_str());
}

TextField::TextField(std::string text)
    : TextField(std::move(text), TextFieldParamsBuilder())
{
}

void TextField::onTextChanged(_lv_event_t *event)
{
    std::string text = lv_textarea_get_text(((lv_obj_t *)event->target));
    if (((TextField *)event->user_data)->_text != text)
    {
        ((TextField *)event->user_data)->_text = text;
        // notify controller about changed data, check if controller exists
        if (auto userController = ((TextField *)event->user_data)->_controller.lock())
        {
            if (auto textEditController = std::dynamic_pointer_cast<TextEditingController>(userController))
            {
                textEditController->onTextChange(text);
            }
        }
    }
}

void TextField::render()
{
}

void TextField::setController(const std::shared_ptr<Controller> &userController)
{
    auto userTextController = std::static_pointer_cast<TextController>(userController);
    _controller = userTextController;
    userTextController->setWidget(
        std::static_pointer_cast<TextControllable>(std::static_pointer_cast<Text>(shared_from_this())));
}

std::string TextField::getText()
{
    if (lvglObject.getObject())
    {
        UniqueLock lock(LvglPlus::lvglMutex());
        _text = lv_textarea_get_text(lvglObject.getObject());
    }
    return _text;
}

void TextField::setText(std::string text)
{
    _text = text;
    if (lvglObject.getObject())
    {
        UniqueLock lock(LvglPlus::lvglMutex());
        lv_textarea_set_text(lvglObject.getObject(), text.c_str());
    }
}

} // namespace lvglplus
} // namespace softeq