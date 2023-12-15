#include "widgets/text.hh"
#include "lvgl_plus.hh"
#include <iostream>

namespace softeq
{
namespace lvglplus
{

const short TextParams::defaultHeight = LV_SIZE_CONTENT;
const short TextParams::defaultWidth = LV_SIZE_CONTENT;

Text::Text(std::string text, const TextParamsBuilder &params)
    : Widget("Text", params.getParams(), lv_label_create)
    , TextOwner(this->lvglObject.getObject())
{
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::noChild);
    _text = std::move(text);
    UniqueLock locker(LvglPlus::lvglMutex());
    auto object = lvglObject.getObject();
    lv_label_set_text(object, _text.c_str());
    lv_label_set_long_mode(object, LV_LABEL_LONG_WRAP); /*Break the long lines*/
    lv_label_set_recolor(object, true);                 /*Enable re-coloring by commands in the text*/
}

Text::Text(std::string text)
    : Text(std::move(text), TextParamsBuilder())
{
}

Text::Text()
    : Text("Text")
{
}

void Text::render()
{
}

void Text::setController(const std::shared_ptr<Controller> &userController)
{
    auto userTextController = std::static_pointer_cast<TextController>(userController);
    _controller = userTextController;
    userTextController->setWidget(
        std::static_pointer_cast<TextControllable>(std::static_pointer_cast<Text>(shared_from_this())));
}

std::string Text::getText()
{
    if (lvglObject.getObject())
    {
        UniqueLock lock(LvglPlus::lvglMutex());
        _text = lv_label_get_text(lvglObject.getObject());
    }
    return _text;
}

void Text::setText(std::string text)
{
    _text = text;
    if (lvglObject.getObject())
    {
        UniqueLock lock(LvglPlus::lvglMutex());
        lv_label_set_text(lvglObject.getObject(), text.c_str());
    }
}

} // namespace lvglplus
} // namespace softeq