#include "lvgl_plus.hh"
#include "lvgl.h"
#include "lvgl_display.hh"
#include "widgets/message_box.hh"
#include <memory>
#include <thread>

namespace softeq
{
namespace lvglplus
{
std::recursive_mutex LvglPlus::_lvglMutex;
uint8_t LvglPlus::DisplayContainer::idCounter = 0;
std::list<std::shared_ptr<LvglPlus::DisplayContainer>> LvglPlus::_displayContainer;

bool LvglPlus::_lvglInitialized = false;
bool LvglPlus::_exitFlag = false;

LvglPlus::~LvglPlus()
{
    lv_deinit();
}

void LvglPlus::stopApp()
{
    _exitFlag = true;
}

void LvglPlus::runApp(bool drawOnce)
{
    _exitFlag = drawOnce;
    for (const auto &displayContainer : _displayContainer)
    {
        static_cast<Renderable *>(displayContainer->rootWidget.get())->renderWithChildren();
    }
    UniqueLock locker(lvglMutex());
    locker.unlock();
    do
    {
        locker.lock();
        lv_task_handler();
        locker.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    } while (!_exitFlag);
}

void LvglPlus::initialize()
{
    /*Initialize LVGL*/
    if (!_lvglInitialized)
    {
        lv_init();
        _lvglInitialized = true;
    }
}

void LvglPlus::setScreen(const std::shared_ptr<Widget> &screen, std::shared_ptr<LvglDisplay> display)
{
    if (!display)
    {
        display = findDisplay(0);
    }
    if (!display)
    {
        // todo throw Exception "No screen found"
    }
    else
    {
        findDisplayContainer(display)->rootWidget->replaceChild(screen);
    }
}

std::shared_ptr<LvglDisplay> LvglPlus::findDisplay(uint8_t id)
{
    for (const auto &displayContainer : _displayContainer)
    {
        if (displayContainer->id == id)
        {
            return displayContainer->lvglDisplay;
        }
    }
    return nullptr;
}

std::shared_ptr<LvglDisplay> LvglPlus::findDisplay(const std::shared_ptr<Widget> &root)
{
    for (const auto &displayContainer : _displayContainer)
    {
        if (displayContainer->rootWidget == root)
        {
            return displayContainer->lvglDisplay;
        }
    }
    return nullptr;
}

std::shared_ptr<Theme> LvglPlus::findCustomTheme(const std::shared_ptr<Widget> &root)
{
    for (const auto &displayContainer : _displayContainer)
    {
        if (displayContainer->rootWidget == root)
        {
            return displayContainer->theme;
        }
    }
    return nullptr;
}

std::shared_ptr<Widget> LvglPlus::findRoot(const std::shared_ptr<LvglDisplay> &display)
{
    for (const auto &displayContainer : _displayContainer)
    {
        if (displayContainer->lvglDisplay == display)
        {
            return displayContainer->rootWidget;
        }
    }
    return nullptr;
}

std::shared_ptr<LvglPlus::DisplayContainer> LvglPlus::findDisplayContainer(std::shared_ptr<LvglDisplay> display)
{
    if (!display)
    {
        display = findDisplay(0);
    }
    for (auto displayContainer : _displayContainer)
    {
        if (displayContainer->lvglDisplay == display)
        {
            return displayContainer;
        }
    }
    return nullptr;
}

std::shared_ptr<LvglPlus::DisplayContainer> LvglPlus::findDisplayContainer(const std::shared_ptr<Widget> &root)
{
    for (auto displayContainer : _displayContainer)
    {
        if (displayContainer->rootWidget == root)
        {
            return displayContainer;
        }
    }
    return nullptr;
}

void LvglPlus::messageBox(const std::shared_ptr<Widget> &root)
{
    LvglPlus::messageBox(root, MessageBoxParamsBuilder());
}

void LvglPlus::messageBox(const std::shared_ptr<Widget> &root, const MessageBoxParamsBuilder &messageBoxBuilder)
{
    auto rootWidget = root->root();
    auto display = findDisplay(rootWidget);
    if (display)
    {
        auto messageBoxWidget = std::make_shared<MessageBox>(display, messageBoxBuilder);
        std::weak_ptr<MessageBox> messageBoxWidgetWeak = messageBoxWidget;
        messageBoxWidget->setOnExit(
            [rootWidget, messageBoxWidgetWeak]() { rootWidget->removeChild(messageBoxWidgetWeak.lock()); });
        rootWidget->addChild(messageBoxWidget);
    }
}

void LvglPlus::setCustomTheme(const std::shared_ptr<Widget> &root, const std::shared_ptr<Theme> &theme)
{
    auto rootWidget = root->root();
    auto displayContainer = findDisplayContainer(rootWidget);
    if (displayContainer)
    {
        auto display = displayContainer->lvglDisplay;
        displayContainer->theme = theme;
        displayContainer->theme->setDisp(display->lvDisplay());
        theme->apply();
    }
}

void LvglPlus::removeCustomTheme(const std::shared_ptr<Widget> &root)
{
    auto rootWidget = root->root();
    auto displayContainer = findDisplayContainer(rootWidget);
    if ((displayContainer) && (displayContainer->theme))
    {
        displayContainer->theme->remove();
        displayContainer->theme.reset();
    }
}
std::shared_ptr<LvglDisplay> LvglPlus::removeDisplay(lv_disp_t *disp)
{
    for (const auto &displayContainer : _displayContainer)
    {
        if (displayContainer->lvglDisplay->lvDisplay() == disp)
        {
            _displayContainer.remove(displayContainer);
            break;
        }
    }
}
std::shared_ptr<LvglDisplay> LvglPlus::addDisplay(unsigned short width, unsigned short height,
                                                  void (*flushCallback)(struct _lv_disp_drv_t *, const lv_area_t *,
                                                                        lv_color_t *))
{
    auto display = std::make_shared<LvglDisplay>(width, height, flushCallback);
    lv_disp_set_default(display->lvDisplay());
    auto displayContainer = std::make_shared<DisplayContainer>();
    _displayContainer.push_back(displayContainer);
    displayContainer->rootWidget = std::make_shared<RootWidget>(display->lvDisplay());
    displayContainer->lvglDisplay = display;
    return display;
}

std::shared_ptr<LvglDriver> LvglPlus::addMouse(void (*readCallback)(struct _lv_indev_drv_t *, lv_indev_data_t *),
                                               const std::shared_ptr<LvglDisplay> &display)
{
    return registerDriver(LV_INDEV_TYPE_POINTER, readCallback, display);
}
std::shared_ptr<LvglDriver> LvglPlus::addKeyboard(void (*readCallback)(struct _lv_indev_drv_t *, lv_indev_data_t *),
                                                  const std::shared_ptr<LvglDisplay> &display)
{
    return registerDriver(LV_INDEV_TYPE_KEYPAD, readCallback, display);
}
std::shared_ptr<LvglDriver> LvglPlus::addEncoder(void (*readCallback)(struct _lv_indev_drv_t *, lv_indev_data_t *),
                                                 const std::shared_ptr<LvglDisplay> &display)
{
    return registerDriver(LV_INDEV_TYPE_ENCODER, readCallback, display);
}
std::shared_ptr<LvglDriver> LvglPlus::addButton(void (*readCallback)(struct _lv_indev_drv_t *, lv_indev_data_t *),
                                                const std::shared_ptr<LvglDisplay> &display)
{
    return registerDriver(LV_INDEV_TYPE_BUTTON, readCallback, display);
}
std::shared_ptr<LvglDriver> LvglPlus::addDriver(void (*readCallback)(struct _lv_indev_drv_t *, lv_indev_data_t *),
                                                const std::shared_ptr<LvglDisplay> &display)
{
    return registerDriver(LV_INDEV_TYPE_NONE, readCallback, display);
}

std::shared_ptr<LvglDriver> LvglPlus::registerDriver(lv_indev_type_t type,
                                                     void (*readCallback)(struct _lv_indev_drv_t *, lv_indev_data_t *),
                                                     const std::shared_ptr<LvglDisplay> &display)
{
    auto displayContainer = findDisplayContainer(display);
    auto driver = std::make_shared<LvglDriver>(type, readCallback);
    driver->indev()->driver->disp = display->lvDisplay();
    displayContainer->lvglDriver.push_back(driver);
    return driver;
}

} // namespace lvglplus
} // namespace softeq