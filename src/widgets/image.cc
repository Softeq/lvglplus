#include "widgets/image.hh"
#include "lvgl_plus.hh"
#include "widget_containers/multi_child_widget.hh"

namespace softeq
{
namespace lvglplus
{

const short ImageParams::defaultHeight = LV_SIZE_CONTENT;
const short ImageParams::defaultWidth = LV_SIZE_CONTENT;

Image::~Image() = default;

Image::Image(const ImageParamsBuilder &params)
    : Widget("Image", params.getParams(), lv_img_create)
{
    auto imageParams = params.getButtonParams();
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::singleChild);
    setAngle(imageParams.angle);
    setZoom(imageParams.zoomPercent);
    setMask(imageParams.mask);
}

Image::Image()
    : Image(ImageParamsBuilder())
{
}

void Image::render()
{
}

void Image::setImage(const std::string &pathToImage)
{
    std::string pathPng = std::string(1, LV_FS_STDIO_LETTER) + ":" + pathToImage;
    auto lvObject = lvglObject.getObject();
    UniqueLock locker(LvglPlus::lvglMutex());
    lv_img_set_src(lvObject, pathPng.c_str());
}

void Image::setZoom(unsigned short percent)
{
    UniqueLock locker(LvglPlus::lvglMutex());
    lv_img_set_zoom(lvglObject.getObject(), percent * 256 / 100);
}

void Image::setAngle(float angle)
{
    auto convertedAngle = static_cast<short>(angle * 10);
    UniqueLock locker(LvglPlus::lvglMutex());
    lv_img_set_angle(lvglObject.getObject(), convertedAngle);
}

void Image::setMask(const Color &mask)
{
    setMask(mask.r, mask.g, mask.b, mask.a);
}

void Image::setMask(unsigned char r, unsigned char g, unsigned char b, unsigned char opacity)
{
    auto lvObj = lvglObject.getObject();
    UniqueLock locker(LvglPlus::lvglMutex());
    lv_obj_set_style_img_recolor_opa(lvObj, opacity, 0);
    lv_obj_set_style_img_recolor(lvObj, lv_color_make(r, g, b), 0);
}

} // namespace lvglplus
} // namespace softeq