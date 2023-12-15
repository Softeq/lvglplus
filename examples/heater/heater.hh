#ifndef LVGL_HEATER_HH
#define LVGL_HEATER_HH

#include "controllers/text_editing_controller.hh"
#include "navigation_bloc/navigation_bloc.hh"
#include "widgets/layouts/column.hh"
#include "widgets/layouts/container.hh"
#include <sstream>

namespace slvglplus = softeq::lvglplus;

class Heater
{
public:
    Heater();
    std::shared_ptr<slvglplus::Container> root;
    std::shared_ptr<NavigationBloc> navigationBloc;
    void startTemperatureSimulator();
    void stopTemperatureSimulator();

    static const int simulationDelayMs;
    static const float adjTempTolerance;
    static const float simulationInTempStep;
    static const float simulationOutTempStep;
    static const float simulationOutTempMaxValue;
    static const float simulationOutTempMinValue;

private:
    float inTemperature = 18;
    float outTemperature = 13;
    float adjInsideTemp = 20;
    std::shared_ptr<slvglplus::TextEditingController> insideTemp;
    std::shared_ptr<slvglplus::TextEditingController> insideTempCal;
    std::shared_ptr<slvglplus::TextEditingController> outsideTemp;
    static std::stringstream temperatureStringStream;
    static void temperatureSimulate(Heater *);
    bool allowTemperatureSimalate = false;
};

#endif // LVGL_HEATER_HH
