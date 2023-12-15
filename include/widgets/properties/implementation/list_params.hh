#ifndef LVGL_LIST_PARAMS_HH
#define LVGL_LIST_PARAMS_HH

#include "widget.hh"
#include "widget_containers/multi_child_widget.hh"

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for the List widget
struct ListParams
{
    static const short defaultWidth;
    static const short defaultHeight;
};

/*
 * The Builder class is used to set the ListParams for the List widget that can be passed to the List constructor.
 * The Builder's setters return a reference to the ListParamsBuilder object.
 */
template <typename T>
class ListParamsBuilderBase : public WidgetParamsBuilder<T>
{
protected:
    ListParams _listParams;

public:
    const ListParams &getListParams() const
    {
        return _listParams;
    }

    ListParamsBuilderBase()
        : WidgetParamsBuilder<T>()
    {
        this->width(ListParams::defaultWidth);
        this->height(ListParams::defaultHeight);
        this->padding(Padding(0).row(0));
    }
};

class ListParamsBuilder : public ListParamsBuilderBase<ListParamsBuilder>
{
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_LIST_PARAMS_HH
