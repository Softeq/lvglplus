#include "widgets/tabview.hh"
#include <string>

namespace softeq
{
namespace lvglplus
{

const short TabviewParams::defaultHeight = 100;
const short TabviewParams::defaultWidth = LV_PCT(100);
const std::string TabviewParams::defaultText;
using namespace std::placeholders;

Tabview::Tabview(const TabviewParamsBuilder &params)
    : Widget("Tabview", params.getParams(),
             std::bind(lv_tabview_create, _1, static_cast<uint8_t>(params.getTabviewParams().menuPosition),
                       static_cast<uint8_t>(params.getTabviewParams().tabSize)))
    , IndicatorOwner(lvglObject.getObject())
{
    auto tabviewParams = params.getTabviewParams();
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::multiChild);
}

Tabview::Tabview()
    : Tabview(TabviewParamsBuilder())
{
}

void Tabview::render()
{
}

std::shared_ptr<Widget> Tabview::addChild(const std::string &title, const std::shared_ptr<Widget> &childWidget)
{
    auto tab = lv_tabview_add_tab(lvglObject.getObject(), title.c_str());
    Widget::addChild(childWidget);
    childWidget->lvglObject.setParent(tab);
    return childWidget;
}

Tabview::~Tabview() = default;

} // namespace lvglplus
} // namespace softeq