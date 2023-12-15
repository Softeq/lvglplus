#include "heater.hh"
#include "main_page.hh"
#include "settings_page.hh"
#include "widgets/bloc_widget_builder.hh"
#include "widgets/layouts/column.hh"
#include "widgets/text.hh"
#include <iomanip>
#include <iostream>

using namespace softeq::lvglplus;
static bool outsideDirUp = true;
static const int displayTempPrecision = 1;
std::stringstream Heater::temperatureStringStream;

const int Heater::simulationDelayMs = 100;
const float Heater::adjTempTolerance = 0.01;
const float Heater::simulationInTempStep = 0.1;
const float Heater::simulationOutTempStep = 0.1;
const float Heater::simulationOutTempMaxValue = 14.6;
const float Heater::simulationOutTempMinValue = 11.2;

void Heater::temperatureSimulate(Heater *heater)
{
    while (heater->allowTemperatureSimalate)
    {
        // for example, every 100ms temperature sensors send values
        std::this_thread::sleep_for(std::chrono::milliseconds(simulationDelayMs));
        // simulation heater to get variable temperature
        if (heater->inTemperature < heater->adjInsideTemp - adjTempTolerance)
        {
            heater->inTemperature += simulationInTempStep;
        }
        if (heater->inTemperature > heater->adjInsideTemp + adjTempTolerance)
        {
            heater->inTemperature -= simulationInTempStep;
        }
        // simulation outside temperature floating
        if (heater->outTemperature > simulationOutTempMaxValue)
        {
            outsideDirUp = false;
        }
        if (heater->outTemperature < simulationOutTempMinValue)
        {
            outsideDirUp = true;
        }
        if (outsideDirUp)
        {
            heater->outTemperature += simulationOutTempStep;
        }
        else
        {
            heater->outTemperature -= simulationOutTempStep;
        }
        // convert float inside temperature to string and send it to the controller
        std::stringstream().swap(temperatureStringStream);
        temperatureStringStream << std::fixed << std::setprecision(displayTempPrecision) << heater->inTemperature;
        heater->insideTemp->setText(temperatureStringStream.str());
        // convert float outside temperature to string and send it to the controller
        std::stringstream().swap(temperatureStringStream);
        temperatureStringStream << std::fixed << std::setprecision(displayTempPrecision) << heater->outTemperature;
        heater->outsideTemp->setText(temperatureStringStream.str());
    }
}

Heater::Heater()
{
    navigationBloc = std::make_shared<NavigationBloc>();
    root = std::make_shared<Container>();
    root->setDiagnosticName("root");
    insideTemp = std::make_shared<TextEditingController>("???");
    outsideTemp = std::make_shared<TextEditingController>("???");
    std::stringstream().swap(temperatureStringStream);
    temperatureStringStream << std::fixed << std::setprecision(displayTempPrecision) << adjInsideTemp;
    insideTempCal = std::make_shared<TextEditingController>(temperatureStringStream.str());
    // set inside adjusted temperature controller to update float temperature when text is changed
    insideTempCal->setOnChangeHandler(
        [this](const std::string &tempStr) { adjInsideTemp = ::strtof(tempStr.c_str(), nullptr); });

    auto window = std::make_shared<BlocWidgetBuilder<NavigationBloc, NavigationState>>(
        navigationBloc, [&](const std::shared_ptr<NavigationState> &state) -> std::shared_ptr<Widget> {
            if (auto stateMainPage = navigationBloc->getIf<NavigationMainPageState>(state))
            {
                return std::make_shared<MainPage>(navigationBloc, stateMainPage->title, insideTemp, outsideTemp,
                                                  insideTempCal);
            }

            if (auto stateSettingsPage = navigationBloc->getIf<NavigationSettingsPageState>(state))
            {
                return std::make_shared<SettingsPage>(navigationBloc, stateSettingsPage->title, insideTempCal);
            }

            return nullptr;
        });

    root->addChild(window);
}

void Heater::startTemperatureSimulator()
{
    allowTemperatureSimalate = true;
    // start temperature scanning simulator in a separate thread
    std::thread temperatureSimulator(temperatureSimulate, this);
    temperatureSimulator.detach();
}
void Heater::stopTemperatureSimulator()
{
    allowTemperatureSimalate = false;
}