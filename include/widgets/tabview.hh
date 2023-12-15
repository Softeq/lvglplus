#ifndef LVGL_PLUS_TABVIEW_HH
#define LVGL_PLUS_TABVIEW_HH

#include "lvgl_plus.hh"
#include "properties/implementation/tabview_params.hh"
#include "widget.hh"
#include <utility>

namespace softeq
{
namespace lvglplus
{

class Tabview : public Widget, public IndicatorOwner
{
public:
    Tabview();
    explicit Tabview(const TabviewParamsBuilder &params);
    ~Tabview() override;

    std::shared_ptr<Widget> addChild(const std::string &title, const std::shared_ptr<Widget> &childWidget);
    void openTab(unsigned int id)
    {
        lv_tabview_set_act(lvglObject.getObject(), id, LV_ANIM_ON);
    }

private:
    std::shared_ptr<Widget> addChild(const std::shared_ptr<Widget> &childWidget) override
    {
        return childWidget;
    }
    void render() override;
};

} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_TABVIEW_HH
