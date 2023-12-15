#ifndef LVGL_PLUS_RENDERABLE_HH
#define LVGL_PLUS_RENDERABLE_HH
namespace softeq
{
namespace lvglplus
{

class Renderable
{
public:
    virtual void render() = 0;
    virtual void renderWithChildren() = 0;

protected:
    virtual ~Renderable() = default;
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_PLUS_RENDERABLE_HH
