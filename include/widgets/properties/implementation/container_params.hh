#ifndef LVGL_CONTAINER_PARAMS_HH
#define LVGL_CONTAINER_PARAMS_HH

#include "lvgl.h"
#include "widget.hh"
#include "widget_containers/single_child_widget.hh"

namespace softeq
{
namespace lvglplus
{

// The structure to hold parameters for the Container widget
struct ContainerParams
{
    static const short defaultWidth;
    static const short defaultHeight;
    bool miltiChild = false;
};

/*
 * The Builder class is used to set the ContainerParams for the Container widget that can be passed to the Container
 * constructor.
 * The Builder's setters return a reference to the ContainerParamsBuilder object.
 */
template <typename T>
class ContainerParamsBuilderBase : public WidgetParamsBuilder<T>
{
protected:
    ContainerParams _containerParams;

public:
    const ContainerParams &getContainerParams() const
    {
        return _containerParams;
    }
    ContainerParamsBuilderBase(const WidgetParams &widgetParams)
    {
        this->_widgetParams = widgetParams;
    }

    ContainerParamsBuilderBase()
    {
        this->width(ContainerParams::defaultWidth);
        this->height(ContainerParams::defaultHeight);
    }

    T &multiChild(bool enable)
    {
        _containerParams.miltiChild = enable;
        return this->retVal();
    }
};

class ContainerParamsBuilder : public ContainerParamsBuilderBase<ContainerParamsBuilder>
{
public:
    ContainerParamsBuilder(const WidgetParams &widgetParams)
        : ContainerParamsBuilderBase<ContainerParamsBuilder>(widgetParams)
    {
    }
    ContainerParamsBuilder()
        : ContainerParamsBuilderBase<ContainerParamsBuilder>()
    {
    }
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_CONTAINER_PARAMS_HH
