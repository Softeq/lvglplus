#ifndef LVGL_PLUS_TEST_FIXTURE_HH
#define LVGL_PLUS_TEST_FIXTURE_HH

#include "helpers/sdl/sdl_drv.hh"
#include "lvgl_plus.hh"
#include "widget.hh"

namespace slvglplus = softeq::lvglplus;

struct Gradient
{
    bool enabled = false;
    bool vertical = true;
    long color = 0x000000;
};

// The structure to hold parameters for the widget
struct WidgetTestParams
{
    short width = 200;
    short height = 100;
    short borderWidth = 3;
    long borderColor = 0x440077;
    unsigned short opacity = 66;
    long backgroundColor = 0x443322;
    unsigned short backgroundOpacity = 160;
    slvglplus::Padding padding = slvglplus::Padding(1, 2, 3, 4, 5, 6);
    slvglplus::Alignment alignment = slvglplus::Alignment::bottomCenter;
    slvglplus::Offset offset = slvglplus::Offset(5, -2);
    Gradient gradient;
    short radius = 4;
    int zIndex = 0;
};

// The structure to hold parameters for the widget with text
struct TextTestParams
{
    long fontColor = 0xff0011;
    int fontSize = 16;
    slvglplus::TextAlignment fontAlign = slvglplus::TextAlignment::right;
};

// The structure to hold parameters for the widget with indicator
struct IndicatorTestParams
{
    long indicatorColor = 0x112211;
    short indicatorBorderWidth = 7;
    long indicatorBorderColor = 0x774411;
};

// The structure to hold parameters for the widget with knob
struct KnobTestParams
{
    short knobSize = 10;
    long knobColor = 0x112233;
    short knobBorderWidth = 4;
    long knobBorderColor = 0x332211;
};

class TestParams
{
public:
    static TestParams &getInstance()
    {
        static TestParams instance;
        static bool initialized = false;
        if (!initialized)
        {
            slvglplus::LvglPlus::initialize();
            addSdlPeripherals(horRes, verRes);
        }
        initialized = true;
        return instance;
    }
    static const int horRes = 200;
    static const int verRes = 200;
};

#endif // LVGL_PLUS_TEST_FIXTURE_HH
