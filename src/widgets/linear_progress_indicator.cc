#include "widgets/linear_progress_indicator.hh"
#include "lvgl_plus.hh"

namespace softeq
{
namespace lvglplus
{

const short LinearProgressIndicatorParams::defaultHeight = 15;
const short LinearProgressIndicatorParams::defaultWidth = 200;

LinearProgressIndicator::~LinearProgressIndicator() = default;

LinearProgressIndicator::LinearProgressIndicator(const LinearProgressIndicatorParamsBuilder &params)
    : Widget("LinearProgressIndicator", params.getParams(), lv_bar_create)
    , RangeOwner()
    , IndicatorOwner(lvglObject.getObject())
{
    auto barParams = params.getLinearProgressIndicatorParams();
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::singleChild);
    applyRangeParams(params.getRangeParams());
}

LinearProgressIndicator::LinearProgressIndicator()
    : LinearProgressIndicator(LinearProgressIndicatorParamsBuilder())
{
}

void LinearProgressIndicator::render()
{
}

int LinearProgressIndicator::min() const
{
    return lv_bar_get_min_value(lvglObject.getObject());
}

int LinearProgressIndicator::max() const
{
    return lv_bar_get_max_value(lvglObject.getObject());
}

int LinearProgressIndicator::value() const
{
    return lv_bar_get_value(lvglObject.getObject());
}

void LinearProgressIndicator::setRange(int min, int max)
{
    UniqueLock locker(LvglPlus::lvglMutex());
    lv_bar_set_range(lvglObject.getObject(), min, max);
}

void LinearProgressIndicator::setValue(int value)
{
    UniqueLock locker(LvglPlus::lvglMutex());
    lv_bar_set_value(lvglObject.getObject(), value, LV_ANIM_OFF);
}

} // namespace lvglplus
} // namespace softeq