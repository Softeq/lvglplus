#include "widgets/led.hh"

namespace softeq
{
namespace lvglplus
{

const short LedParams::defaultHeight = 20;
const short LedParams::defaultWidth = 20;

Led::Led(const LedParamsBuilder &params)
    : Widget("Led", params.getParams(), lv_led_create)
{
    auto ledParams = params.getLedParams();
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::singleChild);
    setColor(ledParams.color);
    if (ledParams.state)
    {
        if (ledParams.brightness != 0)
        {
            setBrightness(ledParams.brightness);
        }
        else
        {
            on();
        }
    }
    else
    {
        off();
    }
}

Led::Led()
    : Led(LedParamsBuilder())
{
}

void Led::render()
{
}

Led::~Led() = default;

} // namespace lvglplus
} // namespace softeq