#ifndef LVGL_SPINBOX_HH
#define LVGL_SPINBOX_HH
#include "text.hh"
#include "widget.hh"
#include "widgets/properties/implementation/spinbox_params.hh"

namespace softeq
{
namespace lvglplus
{

class Spinbox : public Widget, public TextOwner, RangeOwner<SpinboxRangeType>
{
public:
    Spinbox();
    explicit Spinbox(const SpinboxParamsBuilder &params);
    ~Spinbox() override;

    void setOnChangeHandler(std::function<void(void)> handler)
    {
        onChangeCallback = std::move(handler);
    }

    void increment();
    void decrement();
    void setStep(SpinboxRangeType step);
    void setRange(SpinboxRangeType min, SpinboxRangeType max) final;
    void setValue(SpinboxRangeType value) final;
    SpinboxRangeType value() const final;
    void setDigitFormat(unsigned char digits, unsigned char separatorPosition);
    SpinboxRangeType min() const final;
    SpinboxRangeType max() const final;

private:
    SpinboxRangeType _min = 0;
    SpinboxRangeType _max = 0;
    unsigned char _separatorPosition = 0;
    unsigned char _digits = 0;
    static void onChange(_lv_event_t *c);
    std::function<void(void)> onChangeCallback;
    void render() override;
};
} // namespace lvglplus
} // namespace softeq

#endif // LVGL_SPINBOX_HH
