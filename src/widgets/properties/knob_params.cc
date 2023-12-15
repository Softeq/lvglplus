#include "widgets/properties/base/knob_params.hh"

namespace softeq
{
namespace lvglplus
{

void KnobOwner::applyKnobParams(const KnobParams &params)
{
    setKnobColor(params.knobColor);
    setKnobBorderWidth(params.knobBorderWidth);
    setKnobBorderColor(params.knobBorderColor);
    setKnobSize(params.knobSize);
}

} // namespace lvglplus
} // namespace softeq