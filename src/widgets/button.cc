#include "widgets/button.hh"
#include "lvgl_plus.hh"
#include "widget_container.hh"

namespace softeq
{
namespace lvglplus
{

using namespace std::placeholders;

const short ButtonParams::defaultHeight = 35;
const short ButtonParams::defaultWidth = 60;

Button::Button(const ButtonParamsBuilder &params)
    : Widget("Button", params.getParams(), nullptr)
{
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::singleChild);
    onClickCallback = params.getButtonParams().onClickCallback;
    auto buttonParams = params.getButtonParams();
    auto builder = params;
    _lv_obj_t *lvObject;
    UniqueLock locker(LvglPlus::lvglMutex());
    if (buttonParams.hasImage)
    {
        lvObject = lv_imgbtn_create(lv_scr_act());
        /*Darken the button when pressed and make it wider*/
        static lv_style_t style_pr;
        lv_style_init(&style_pr);
        lv_style_set_img_recolor_opa(&style_pr, LV_OPA_30);
        lv_style_set_img_recolor(&style_pr, lv_color_black());
        lv_style_set_transform_width(&style_pr, 5);
        lv_obj_add_style(lvObject, &style_pr, LV_STATE_PRESSED);
        pathLeft = std::string(1, LV_FS_STDIO_LETTER) + ":" + buttonParams.imagePathLeft;
        pathMid = std::string(1, LV_FS_STDIO_LETTER) + ":" + buttonParams.imagePathMid;
        pathRight = std::string(1, LV_FS_STDIO_LETTER) + ":" + buttonParams.imagePathRight;
        lv_imgbtn_set_src(lvObject, LV_IMGBTN_STATE_RELEASED, pathLeft.c_str(), pathMid.c_str(), pathRight.c_str());
        builder.height(lv_obj_get_style_height(lvObject, 0));
        builder.backgroundOpacity(0);
    }
    else
    {
        lvObject = lv_btn_create(lv_scr_act());
    }
    prepareWidget(builder.getParams(), lvObject);
    lv_obj_add_event_cb(lvObject, &Button::onClick, LV_EVENT_CLICKED, this);
}

Button::Button(std::string buttonText, const ButtonParamsBuilder &params)
    : Button(params)
{
    auto textWidget = std::make_shared<Text>(std::move(buttonText),
                                             TextParamsBuilder(params.getButtonParams()).alignment(Alignment::center));
    _textWidget = textWidget;
    this->addChild(textWidget);
    if (width() == ButtonParams::defaultWidth)
    {
        setWidth(LV_SIZE_CONTENT);
    }
}

Button::Button()
    : Button(ButtonParamsBuilder())
{
}

Button::Button(std::string buttonText)
    : Button(std::move(buttonText), ButtonParamsBuilder())
{
}

void Button::onClick(_lv_event_t *event)
{
    if (((Button *)event->user_data)->onClickCallback)
    {
        ((Button *)event->user_data)->onClickCallback();
    }
}

void Button::render()
{
}

Button::~Button() = default;
} // namespace lvglplus
} // namespace softeq