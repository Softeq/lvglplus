#ifndef LVGL_PLUS_LIST_WHEEL_SCROLL_VIEW_PARAMS_HH
#define LVGL_PLUS_LIST_WHEEL_SCROLL_VIEW_PARAMS_HH

#include "widget.hh"
#include "widgets/properties/base/indicator_params.hh"
#include "widgets/properties/base/text_basic_params.hh"
#include <utility>

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for the LinearProgressIndicator widget
struct ListWheelScrollViewParams : public TextBasicParams
{
    static const short defaultWidth;
    static const short defaultHeight;
    std::function<void(void)> onChangeCallback;
};

/*
 * The Builder class is used to set the ListWheelScrollViewParams for the ListWheelScrollView widget that can be passed
 * to the ListWheelScrollView constructor.
 * The Builder's setters return a reference to the ListWheelScrollViewParamsBuilder object.
 */
template <typename T>
class ListWheelScrollViewParamsBuilderBase : public WidgetParamsBuilder<T>,
                                             public TextBasicParamsBuilder<T>,
                                             public IndicatorParamsBuilder<T>
{
protected:
    ListWheelScrollViewParams _listWheelScrollViewParams;

public:
    const ListWheelScrollViewParams &getListWheelScrollViewParams() const
    {
        return _listWheelScrollViewParams;
    }

    ListWheelScrollViewParamsBuilderBase()
        : WidgetParamsBuilder<T>()
        , TextBasicParamsBuilder<T>()
        , IndicatorParamsBuilder<T>()
    {
        this->width(ListWheelScrollViewParams::defaultWidth);
        this->height(ListWheelScrollViewParams::defaultHeight);
    }

    T &onChangeHandler(std::function<void(void)> handler)
    {
        _listWheelScrollViewParams.onChangeCallback = std::move(handler);
        return this->retVal();
    }
};

class ListWheelScrollViewParamsBuilder : public ListWheelScrollViewParamsBuilderBase<ListWheelScrollViewParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_LIST_WHEEL_SCROLL_VIEW_PARAMS_HH
