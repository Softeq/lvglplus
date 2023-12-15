#include "heater.hh"
#include "helpers/sdl/sdl_drv.hh"
#include "lvgl_plus.hh"

using namespace softeq::lvglplus;

int main(int argc, char *argv[])
{
    LvglPlus::initialize();
    addSdlPeripherals(LVGL_SCREEN_WIDTH, LVGL_SCREEN_HEIGHT);
    Heater heater;
    heater.startTemperatureSimulator();
    LvglPlus::setScreen(heater.root);
    LvglPlus::runApp();
    return 0;
}