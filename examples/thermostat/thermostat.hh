#ifndef LVGL_HEATER_HH
#define LVGL_HEATER_HH

#include "parameters.hh"
#include "widgets/layouts/container.hh"
#include <communication/streams/include/stream_controller.hh>

namespace slvglplus = softeq::lvglplus;
namespace sstreams = softeq::streams;

enum class PageState
{
    MainPage,
    SettingsPage
};

class Thermostat
{
public:
    Thermostat();
    std::shared_ptr<slvglplus::Container> root;
    std::shared_ptr<sstreams::StreamSubscription<PageState>> pageSubscription;

private:
    Parameters parameters;
    std::shared_ptr<sstreams::StreamEventLoop> streamEventLoop;
    std::shared_ptr<sstreams::StreamController<PageState>> pageController;
};

#endif // LVGL_HEATER_HH
