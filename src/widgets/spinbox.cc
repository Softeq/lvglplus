#include "widgets/spinbox.hh"
#include "lvgl_plus.hh"
#include "widget_container.hh"
#include <cmath>

namespace softeq
{
namespace lvglplus
{

using namespace std::placeholders;

const short SpinboxParams::defaultHeight = LV_SIZE_CONTENT;
const short SpinboxParams::defaultWidth = 60;

Spinbox::Spinbox(const SpinboxParamsBuilder &params)
    : Widget("Spinbox", params.getParams(), lv_spinbox_create)
    , TextOwner(this->lvglObject.getObject())
{
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::singleChild);
    onChangeCallback = params.getSpinboxParams().onChangeCallback;

    auto spinboxParams = params.getSpinboxParams();
    applyRangeParams(params.getRangeParams());
    setDigitFormat(spinboxParams.digits, spinboxParams.separatorPosition);
    UniqueLock locker(LvglPlus::lvglMutex());
    lv_obj_add_event_cb(lvglObject.getObject(), &Spinbox::onChange, LV_EVENT_VALUE_CHANGED, this);
}

Spinbox::Spinbox()
    : Spinbox(SpinboxParamsBuilder())
{
}

void Spinbox::onChange(_lv_event_t *event)
{
    if (((Spinbox *)event->user_data)->onChangeCallback)
    {
        ((Spinbox *)event->user_data)->onChangeCallback();
    }
}

void Spinbox::render()
{
}

SpinboxRangeType Spinbox::value() const
{
    UniqueLock locker(LvglPlus::lvglMutex());
    return static_cast<float>(lv_spinbox_get_value(lvglObject.getObject()) / std::pow(10, _separatorPosition));
}

void Spinbox::setRange(SpinboxRangeType min, SpinboxRangeType max)
{
    _min = min;
    _max = max;
    auto indentCoef = std::pow(10, _separatorPosition);
    int spinboxMin = static_cast<int>(min * indentCoef);
    int spinboxMax = static_cast<int>(max * indentCoef);
    UniqueLock locker(LvglPlus::lvglMutex());
    lv_spinbox_set_range(lvglObject.getObject(), spinboxMin, spinboxMax);
}

void Spinbox::setValue(SpinboxRangeType value)
{
    UniqueLock locker(LvglPlus::lvglMutex());
    float convertedValue = value * static_cast<float>(std::pow(10.0, _separatorPosition));
    lv_spinbox_set_value(lvglObject.getObject(), static_cast<int>(convertedValue));
}

void Spinbox::increment()
{
    UniqueLock locker(LvglPlus::lvglMutex());
    lv_spinbox_increment(lvglObject.getObject());
}

void Spinbox::decrement()
{
    UniqueLock locker(LvglPlus::lvglMutex());
    lv_spinbox_decrement(lvglObject.getObject());
}

void Spinbox::setStep(SpinboxRangeType step)
{
    UniqueLock locker(LvglPlus::lvglMutex());
    float convertedStep = step * static_cast<float>(std::pow(10.0, _separatorPosition));
    lv_spinbox_set_step(lvglObject.getObject(), static_cast<int>(convertedStep));
}

void Spinbox::setDigitFormat(unsigned char digits, unsigned char separatorPosition)
{
    _separatorPosition = separatorPosition;
    _digits = digits;
    UniqueLock locker(LvglPlus::lvglMutex());
    lv_spinbox_set_digit_format(lvglObject.getObject(), digits, digits - separatorPosition);
    setRange(_min, _max);
}

SpinboxRangeType Spinbox::min() const
{
    return _min;
}

SpinboxRangeType Spinbox::max() const
{
    return _max;
}

Spinbox::~Spinbox() = default;
} // namespace lvglplus
} // namespace softeq