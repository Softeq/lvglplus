#ifndef LVGL_PLUS_WAIT_FOR_HH
#define LVGL_PLUS_WAIT_FOR_HH
#include "lvgl_plus.hh"
#include "widget.hh"

bool waitForCondition(const std::function<bool()> &condition, int timeoutCounterMs = 1000);
bool waitForRender(std::shared_ptr<softeq::lvglplus::Widget> widget);

#endif // LVGL_PLUS_WAIT_FOR_HH
