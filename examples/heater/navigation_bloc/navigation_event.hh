#ifndef LVGL_NAVIGATION_EVENT_HH
#define LVGL_NAVIGATION_EVENT_HH

class NavigationEvent
{
public:
    virtual ~NavigationEvent() = default;
};

class OpenMainPageEvent : public NavigationEvent
{
};
class OpenSettingsPageEvent : public NavigationEvent
{
};

#endif // LVGL_NAVIGATION_EVENT_HH
