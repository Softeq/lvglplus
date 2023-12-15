#include "thermostat.hh"
#include "include/widgets/stream_widget_builder.hh"
#include "lvgl_plus.hh"
#include "main_page.hh"
#include "settings_page.hh"

using namespace softeq::lvglplus;
using namespace softeq::streams;

Thermostat::Thermostat()
{
    streamEventLoop = std::make_shared<StreamEventLoop>();
    pageController = std::make_shared<StreamController<PageState>>(streamEventLoop);

    root = std::make_shared<Container>();

    auto window = std::make_shared<StreamWidgetBuilder<PageState>>(
        PageState::MainPage, pageController->stream(), [&](const PageState &pageState) -> std::shared_ptr<Widget> {
            switch (pageState)
            {
            case PageState::MainPage:
                return std::make_shared<MainPage>(pageController, &parameters);
            case PageState::SettingsPage:
                return std::make_shared<SettingsPage>(pageController, &parameters);
            default:
                throw std::runtime_error("StreamWidgetBuilder: page not found");
            }
        });

    root->addChild(window);
}