#include "helpers/sdl/sdl_drv.hh"
#include "include/lvgl_plus.hh"
#include "thermostat.hh"

using namespace softeq::lvglplus;

int main(int argc, char *argv[])
{
    LvglPlus::initialize();
    addSdlPeripherals(LVGL_SCREEN_WIDTH, LVGL_SCREEN_HEIGHT);
    Thermostat thermostat;
    LvglPlus::setScreen(thermostat.root);
    LvglPlus::runApp();
    return 0;
}
