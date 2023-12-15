#ifndef LVGL_PLUS_PARAMETERS_HH
#define LVGL_PLUS_PARAMETERS_HH

class Parameters
{
public:
    short temperatureSet = 23;
    int fanSpeed = 2;
    bool quietEnable = false;
    bool turboEnable = true;
    int fanSwing = 1;
    bool bedroomEnable = true;
    bool kitchenEnable = false;
    bool livingroomEnable = false;
    int mode = 1;
};

#endif //LVGL_PLUS_PARAMETERS_HH
