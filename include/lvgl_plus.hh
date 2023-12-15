#ifndef LVGL_LVGL_PLUS_HH
#define LVGL_LVGL_PLUS_HH

#include "lvgl_display.hh"
#include "lvgl_driver.hh"
#include "root_widget.hh"
#include "widget.hh"
#include "widgets/message_box.hh"
#include "widgets/properties/theme.hh"
#include <list>
#include <mutex>

namespace softeq
{
namespace lvglplus
{

class LvglPlus
{
private:
    class DisplayContainer
    {
    public:
        uint8_t id;
        std::shared_ptr<LvglDisplay> lvglDisplay;
        std::list<std::shared_ptr<LvglDriver>> lvglDriver;
        std::shared_ptr<RootWidget> rootWidget;
        std::shared_ptr<Theme> theme;
        DisplayContainer()
        {
            id = idCounter;
            idCounter++;
        }
        ~DisplayContainer()
        {
            rootWidget.reset();
            lvglDriver.clear();
            lvglDisplay.reset();
        }

    private:
        static uint8_t idCounter;
    };

public:
    ~LvglPlus();
    static void initialize();
    static void runApp(bool drawOnce = false);
    static void stopApp();

    static std::shared_ptr<LvglDisplay>
    addDisplay(unsigned short width, unsigned short height,
               void (*flushCallback)(struct _lv_disp_drv_t *dispDrv, const lv_area_t *area, lv_color_t *colorArray));
    static std::shared_ptr<LvglDriver> addMouse(void (*readCallback)(struct _lv_indev_drv_t *indev_drv,
                                                                     lv_indev_data_t *data),
                                                const std::shared_ptr<LvglDisplay> &display = nullptr);

    static std::shared_ptr<LvglDriver> addKeyboard(void (*readCallback)(struct _lv_indev_drv_t *indev_drv,
                                                                        lv_indev_data_t *data),
                                                   const std::shared_ptr<LvglDisplay> &display = nullptr);

    static std::shared_ptr<LvglDriver> addEncoder(void (*readCallback)(struct _lv_indev_drv_t *indev_drv,
                                                                       lv_indev_data_t *data),
                                                  const std::shared_ptr<LvglDisplay> &display = nullptr);

    static std::shared_ptr<LvglDriver> addButton(void (*readCallback)(struct _lv_indev_drv_t *indev_drv,
                                                                      lv_indev_data_t *data),
                                                 const std::shared_ptr<LvglDisplay> &display = nullptr);

    static std::shared_ptr<LvglDriver> addDriver(void (*readCallback)(struct _lv_indev_drv_t *indev_drv,
                                                                      lv_indev_data_t *data),
                                                 const std::shared_ptr<LvglDisplay> &display = nullptr);

    static std::recursive_mutex &lvglMutex()
    {
        return _lvglMutex;
    }

    static std::shared_ptr<LvglDisplay> findDisplay(const std::shared_ptr<Widget> &root);
    static std::shared_ptr<LvglDisplay> findDisplay(uint8_t id);
    static std::shared_ptr<Widget> findRoot(const std::shared_ptr<LvglDisplay> &display);
    static void setScreen(const std::shared_ptr<Widget> &screen, std::shared_ptr<LvglDisplay> display = nullptr);
    static void messageBox(const std::shared_ptr<Widget> &root, const MessageBoxParamsBuilder &messageBoxBuilder);
    static void messageBox(const std::shared_ptr<Widget> &root);
    static void setCustomTheme(const std::shared_ptr<Widget> &root, const std::shared_ptr<Theme> &theme);
    static void removeCustomTheme(const std::shared_ptr<Widget> &root);
    static std::shared_ptr<Theme> findCustomTheme(const std::shared_ptr<Widget> &root);

    static std::shared_ptr<LvglDisplay> removeDisplay(lv_disp_t *disp);

private:
    LvglPlus() = delete;
    static std::list<std::shared_ptr<DisplayContainer>> _displayContainer;
    static std::recursive_mutex _lvglMutex;
    static bool _exitFlag;
    static bool _lvglInitialized;
    static std::shared_ptr<LvglDriver> registerDriver(lv_indev_type_t type,
                                                      void (*readCallback)(_lv_indev_drv_t *, lv_indev_data_t *),
                                                      const std::shared_ptr<LvglDisplay> &display);
    static std::shared_ptr<DisplayContainer> findDisplayContainer(std::shared_ptr<LvglDisplay> display);
    static std::shared_ptr<DisplayContainer> findDisplayContainer(const std::shared_ptr<Widget> &root);
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_LVGL_PLUS_HH
