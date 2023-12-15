#ifndef LVGL_TABVIEW_PARAMS_HH
#define LVGL_TABVIEW_PARAMS_HH

#include "widget.hh"
#include "widget_containers/multi_child_widget.hh"
#include "widgets/properties/base/indicator_params.hh"
#include "widgets/properties/base/text_basic_params.hh"

namespace softeq
{
namespace lvglplus
{

enum class TabviewPosition
{
    Left = (1 << 0),
    Right = (1 << 1),
    Top = (1 << 2),
    Bottom = (1 << 3),
};

// The structure to hold parameters for the Tabview widget
struct TabviewParams
{
    static const short defaultWidth;
    static const short defaultHeight;
    static const std::string defaultText;

    TabviewPosition menuPosition = TabviewPosition::Top;
    unsigned int tabSize = 40;
};

/*
 * The Builder class is used to set the TabviewParams for the Tabview widget that can be passed to the Tabview
 * constructor.
 * The Builder's setters return a reference to the TabviewParamsBuilder object.
 */
template <typename T>
class TabviewParamsBuilderBase : public WidgetParamsBuilder<T>, public IndicatorParamsBuilder<T>
{
protected:
    TabviewParams _tabviewParams;

public:
    const TabviewParams &getTabviewParams() const
    {
        return _tabviewParams;
    }

    TabviewParamsBuilderBase()
        : WidgetParamsBuilder<T>()
        , IndicatorParamsBuilder<T>()
    {
        this->width(TabviewParams::defaultWidth);
        this->height(TabviewParams::defaultHeight);
        //        this->_widgetParams.backgroundOpacity = TabviewParams::defaultBackgroundOpacity;
    }
};

class TabviewParamsBuilder : public TabviewParamsBuilderBase<TabviewParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_TABVIEW_PARAMS_HH
