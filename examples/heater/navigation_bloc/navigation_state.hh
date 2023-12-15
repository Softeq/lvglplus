#ifndef LVGL_NAVIGATION_STATE_HH
#define LVGL_NAVIGATION_STATE_HH

class NavigationState
{
public:
    virtual ~NavigationState() = default;
};

class NavigationMainPageState : public NavigationState
{
public:
    std::string title = "Heater device";
};
class NavigationSettingsPageState : public NavigationState
{
public:
    std::string title = "Settings";
};

#endif // LVGL_NAVIGATION_STATE_HH
