#ifndef LVGL_NAVIGATION_BLOC_HH
#define LVGL_NAVIGATION_BLOC_HH

#include "bloc/bloc.hh"
#include "navigation_event.hh"
#include "navigation_state.hh"

using namespace softeq::lvglplus;

class NavigationBloc : public Bloc<NavigationEvent, NavigationState>
{
public:
    NavigationBloc()
        : Bloc<NavigationEvent, NavigationState>(NavigationMainPageState())
    {
        on<OpenMainPageEvent>([this](OpenMainPageEvent event) { this->emit(NavigationMainPageState()); });
        on<OpenSettingsPageEvent>([this](OpenSettingsPageEvent event) { this->emit(NavigationSettingsPageState()); });
    }

private:
};

#endif // LVGL_NAVIGATION_BLOC_HH
