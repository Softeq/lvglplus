#ifndef LVGL_PLUS_IMAGE_HH
#define LVGL_PLUS_IMAGE_HH

#include "properties/implementation/image_params.hh"

namespace softeq
{
namespace lvglplus
{

class Image : public Widget
{
public:
    Image();
    explicit Image(const ImageParamsBuilder &params);
    ~Image() override;
    void setImage(const std::string &pathToImage);
    void setAngle(float angle);
    void setZoom(unsigned short percent);
    void setMask(unsigned char r, unsigned char g, unsigned char b, unsigned char opacity);
    void setMask(const Color &mask);

private:
    void render() override;
};


} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_IMAGE_HH
