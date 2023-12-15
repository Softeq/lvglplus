#ifndef LVGL_IMAGE_PARAMS_HH
#define LVGL_IMAGE_PARAMS_HH

#include "bitmap.hh"
#include "widget.hh"

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for the Image widget
struct ImageParams
{
    static const short defaultWidth;
    static const short defaultHeight;
    float angle = 0;
    unsigned short zoomPercent = 100;
    Color mask = Color(0, 0, 0, 0);
};

/*
 * The Builder class is used to set the ImageParams for the Image widget that can be passed to the Image constructor.
 * The Builder's setters return a reference to the ImageParamsBuilder object.
 */
template <typename T>
class ImageParamsBuilderBase : public WidgetParamsBuilder<T>
{
protected:
    ImageParams _imageParams;

public:
    const ImageParams &getButtonParams() const
    {
        return _imageParams;
    }

    ImageParamsBuilderBase()
        : WidgetParamsBuilder<T>()
    {
        this->width(ImageParams::defaultWidth);
        this->height(ImageParams::defaultHeight);
    }

    T &angle(float angle)
    {
        _imageParams.angle = angle;
        return this->retVal();
    }

    T &zoom(unsigned short percent)
    {
        _imageParams.zoomPercent = percent;
        return this->retVal();
    }

    T &mask(const Color &mask)
    {
        _imageParams.mask = mask;
        return this->retVal();
    }

    T &mask(unsigned char r, unsigned char g, unsigned char b, unsigned char opacity)
    {
        _imageParams.mask.r = r;
        _imageParams.mask.g = g;
        _imageParams.mask.b = b;
        _imageParams.mask.a = opacity;
        return this->retVal();
    }
};

class ImageParamsBuilder : public ImageParamsBuilderBase<ImageParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_IMAGE_PARAMS_HH
