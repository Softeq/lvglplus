#include "sdl_drv.hh"
#if USE_SDL
#include SDL_INCLUDE_PATH
#include <list>
#include <memory>
#include <utility>

#ifndef SDL_FULLSCREEN
#define SDL_FULLSCREEN 0
#endif
#ifndef KEYBOARD_BUFFER_SIZE
#define KEYBOARD_BUFFER_SIZE SDL_TEXTINPUTEVENT_TEXT_SIZE
#endif

class SdlDrvCfg
{
public:
    std::function<void(void)> removeCb = nullptr;
    ~SdlDrvCfg()
    {
        if (removeCb)
        {
            removeCb();
        }
        SDL_DestroyTexture(windowDrvCfg.texture);
        SDL_DestroyRenderer(windowDrvCfg.renderer);
        SDL_DestroyWindow(windowDrvCfg.window);
    }
    class DisplayDrvCfg
    {
    public:
        lv_disp_drv_t *dispDrv = nullptr;
        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;
        SDL_Texture *texture = nullptr;
        volatile bool sdlRefrQry = false;
#if SDL_DOUBLE_BUFFERED
        uint32_t *tftFrameBufferAct = nullptr;
#else
        uint32_t *tftFrameBuffer = nullptr;
#endif
        uint32_t displayId = 0;
        void create(lv_disp_drv_t *disp);
        void update() const;
    };

    struct MouseDrvCfg
    {
        unsigned short id = 0;
        bool leftButtonDown = false;
        int16_t lastX = 0;
        int16_t lastY = 0;
        explicit MouseDrvCfg(unsigned short id = 0)
            : id(id)
        {
        }
    };
    struct WheelDrvCfg
    {
        unsigned short id = 0;
        int16_t wheelDiff = 0;
        lv_indev_state_t wheelState = LV_INDEV_STATE_RELEASED;
        explicit WheelDrvCfg(unsigned short id = 0)
            : id(id)
        {
        }
    };
    struct KeyboardDrvCfg
    {
        unsigned short id = 0;
        char buf[KEYBOARD_BUFFER_SIZE]{};
        explicit KeyboardDrvCfg(unsigned short id = 0)
            : id(id)
        {
            buf[0] = 0;
        }
    };

    std::unique_ptr<MouseDrvCfg> mouseDrvCfg;
    std::unique_ptr<WheelDrvCfg> wheelDrvCfg;
    std::unique_ptr<KeyboardDrvCfg> keyboardDrvCfg;

    DisplayDrvCfg windowDrvCfg;
    explicit SdlDrvCfg(lv_disp_drv_t *disp, std::function<void(void)> removeCallback)
    {
        removeCb = std::move(removeCallback);
        windowDrvCfg.create(disp);
    }
};
std::shared_ptr<SdlDrvCfg> findSdlDisplay(lv_disp_drv_t *drv);
std::shared_ptr<SdlDrvCfg> findSdlDisplay(uint32_t id);

std::list<std::shared_ptr<SdlDrvCfg>> SdlDisplays;

static void sdlCleanUpSdlDisplay()
{
    for (const auto &SdlDisplay : SdlDisplays)
    {
        SDL_DestroyTexture(SdlDisplay->windowDrvCfg.texture);
        SDL_DestroyRenderer(SdlDisplay->windowDrvCfg.renderer);
        SDL_DestroyWindow(SdlDisplay->windowDrvCfg.window);
    }

    SDL_Quit();
}

volatile bool sdlQuitQry = false;

/**
 * Get the current position and state of the mouse
 * @param indev_drv pointer to the related input device driver
 * @param data store the mouse data here
 */
void sdlMouseRead(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    if (auto sdlDisplay = findSdlDisplay(indev_drv->disp->driver))
    {
        // Store the collected data
        data->point.x = sdlDisplay->mouseDrvCfg->lastX;
        data->point.y = sdlDisplay->mouseDrvCfg->lastY;
        data->state = sdlDisplay->mouseDrvCfg->leftButtonDown ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
    }
}

/**
 * Get encoder (i.e. mouse wheel) ticks difference and pressed state
 * @param indev_drv pointer to the related input device driver
 * @param data store the read data here
 */
void sdlMousewheelRead(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    if (auto sdlDisplay = findSdlDisplay(indev_drv->disp->driver))
    {
        data->state = sdlDisplay->wheelDrvCfg->wheelState;
        data->enc_diff = sdlDisplay->wheelDrvCfg->wheelDiff;
        sdlDisplay->wheelDrvCfg->wheelDiff = 0;
    }
}

/**
 * Get input from the keyboard.
 * @param indev_drv pointer to the related input device driver
 * @param data store the red data here
 */
void sdlKeyboardRead(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    if (auto sdlDisplay = findSdlDisplay(indev_drv->disp->driver))
    {
        static bool dummy_read = false;
        const size_t len = strlen(sdlDisplay->keyboardDrvCfg->buf);

        // Send a release manually
        if (dummy_read)
        {
            dummy_read = false;
            data->state = LV_INDEV_STATE_RELEASED;
            data->continue_reading = len > 0;
        }
        // Send the pressed character
        else if (len > 0)
        {
            dummy_read = true;
            data->state = LV_INDEV_STATE_PRESSED;
            data->key = sdlDisplay->keyboardDrvCfg->buf[0];
            memmove(sdlDisplay->keyboardDrvCfg->buf, sdlDisplay->keyboardDrvCfg->buf + 1, len);
            data->continue_reading = true;
        }
    }
}

void mouseHandler(SDL_Event *event)
{
    if (auto sdlDisplay = findSdlDisplay(event->motion.windowID))
    {
        switch (event->type)
        {
        case SDL_MOUSEBUTTONUP:
            if (event->button.button == SDL_BUTTON_LEFT)
                sdlDisplay->mouseDrvCfg->leftButtonDown = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event->button.button == SDL_BUTTON_LEFT)
            {
                sdlDisplay->mouseDrvCfg->leftButtonDown = true;
                sdlDisplay->mouseDrvCfg->lastX = event->motion.x / SDL_ZOOM;
                sdlDisplay->mouseDrvCfg->lastY = event->motion.y / SDL_ZOOM;
            }
            break;
        case SDL_MOUSEMOTION:
            sdlDisplay->mouseDrvCfg->lastX = event->motion.x / SDL_ZOOM;
            sdlDisplay->mouseDrvCfg->lastY = event->motion.y / SDL_ZOOM;
            break;

        case SDL_FINGERUP:
            sdlDisplay->mouseDrvCfg->leftButtonDown = false;
            sdlDisplay->mouseDrvCfg->lastX = sdlDisplay->windowDrvCfg.dispDrv->hor_res * event->tfinger.x / SDL_ZOOM;
            sdlDisplay->mouseDrvCfg->lastY = sdlDisplay->windowDrvCfg.dispDrv->ver_res * event->tfinger.y / SDL_ZOOM;
            break;
        case SDL_FINGERDOWN:
            sdlDisplay->mouseDrvCfg->leftButtonDown = true;
            sdlDisplay->mouseDrvCfg->lastX = sdlDisplay->windowDrvCfg.dispDrv->hor_res * event->tfinger.x / SDL_ZOOM;
            sdlDisplay->mouseDrvCfg->lastY = sdlDisplay->windowDrvCfg.dispDrv->ver_res * event->tfinger.y / SDL_ZOOM;
            break;
        case SDL_FINGERMOTION:
            sdlDisplay->mouseDrvCfg->lastX = sdlDisplay->windowDrvCfg.dispDrv->hor_res * event->tfinger.x / SDL_ZOOM;
            sdlDisplay->mouseDrvCfg->lastY = sdlDisplay->windowDrvCfg.dispDrv->ver_res * event->tfinger.y / SDL_ZOOM;
            break;
        }
    }
}

/**
 * It is called periodically from the SDL thread to check mouse wheel state
 * @param event describes the event
 */
void mousewheelHandler(SDL_Event *event)
{
    if (auto sdlDisplay = findSdlDisplay(event->motion.windowID))
    {
        switch (event->type)
        {
        case SDL_MOUSEWHEEL:
            // Scroll down (y = -1) means positive encoder turn, so invert it
#ifdef __EMSCRIPTEN__
            /*Escripten scales it wrong*/
            if (event->wheel.y < 0)
                sdlDisplay->wheelDiff++;
            if (event->wheel.y > 0)
                sdlDisplay->wheelDiff--;
#else
            sdlDisplay->wheelDrvCfg->wheelDiff = -event->wheel.y;
#endif
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event->button.button == SDL_BUTTON_MIDDLE)
            {
                sdlDisplay->wheelDrvCfg->wheelState = LV_INDEV_STATE_PRESSED;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event->button.button == SDL_BUTTON_MIDDLE)
            {
                sdlDisplay->wheelDrvCfg->wheelState = LV_INDEV_STATE_RELEASED;
            }
            break;
        default:
            break;
        }
    }
}

/**
 * Convert a SDL key code to it's LV_KEY_* counterpart or return '\0' if it's not a control character.
 * @param sdlKey the key code
 * @return LV_KEY_* control character or '\0'
 */
uint32_t keycodeToCtrlKey(SDL_Keycode sdlKey)
{
    /*Remap some key to LV_KEY_... to manage groups*/

    SDL_Keymod mode = SDL_GetModState();

    switch (sdlKey)
    {
    case SDLK_RIGHT:
    case SDLK_KP_PLUS:
        return LV_KEY_RIGHT;

    case SDLK_LEFT:
    case SDLK_KP_MINUS:
        return LV_KEY_LEFT;

    case SDLK_UP:
        return LV_KEY_UP;

    case SDLK_DOWN:
        return LV_KEY_DOWN;

    case SDLK_ESCAPE:
        return LV_KEY_ESC;

    case SDLK_BACKSPACE:
        return LV_KEY_BACKSPACE;

    case SDLK_DELETE:
        return LV_KEY_DEL;

    case SDLK_KP_ENTER:
    case '\r':
        return LV_KEY_ENTER;

    case SDLK_TAB:
        return (mode & KMOD_SHIFT) ? LV_KEY_PREV : LV_KEY_NEXT;

    case SDLK_PAGEDOWN:
        return LV_KEY_NEXT;

    case SDLK_PAGEUP:
        return LV_KEY_PREV;

    default:
        return '\0';
    }
}

/**
 * Called periodically from the SDL thread, store text input or control characters in the buffer.
 * @param event describes the event
 */
void keyboardHandler(SDL_Event *event)
{
    if (auto sdlDisplay = findSdlDisplay(event->motion.windowID))
    {
        // We only care about SDL_KEYDOWN and SDL_TEXTINPUT events
        switch (event->type)
        {
        case SDL_KEYDOWN: // Button press
        {
            const uint32_t ctrl_key = keycodeToCtrlKey(event->key.keysym.sym);
            if (ctrl_key == '\0')
                return;
            const size_t len = strlen(sdlDisplay->keyboardDrvCfg->buf);
            if (len < KEYBOARD_BUFFER_SIZE - 1)
            {
                sdlDisplay->keyboardDrvCfg->buf[len] = ctrl_key;
                sdlDisplay->keyboardDrvCfg->buf[len + 1] = '\0';
            }
            break;
        }
        case SDL_TEXTINPUT: // Text input
        {
            const size_t len = strlen(sdlDisplay->keyboardDrvCfg->buf) + strlen(event->text.text);
            if (len < KEYBOARD_BUFFER_SIZE - 1)
                strcat(sdlDisplay->keyboardDrvCfg->buf, event->text.text);
        }
        break;
        default:
            break;
        }
    }
}

/**
 * SDL main thread. All SDL related task have to be handled here!
 * It initializes SDL, handles drawing and the mouse.
 */
static void sdlEventHandler(lv_timer_t *t)
{
    (void)t;

    /*Refresh handling*/
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        mouseHandler(&event);
        mousewheelHandler(&event);
        keyboardHandler(&event);

        if ((&event)->type == SDL_WINDOWEVENT)
        {
            switch ((&event)->window.event)
            {
#if SDL_VERSION_ATLEAST(2, 0, 5)
            case SDL_WINDOWEVENT_TAKE_FOCUS:
#endif
            case SDL_WINDOWEVENT_EXPOSED:
                for (const auto &SdlDisplay : SdlDisplays)
                {
                    SdlDisplay->windowDrvCfg.update();
                }
                break;
            default:
                break;
            }
        }
    }

    /*Run until quit event not arrives*/
    if (sdlQuitQry)
    {
        sdlCleanUpSdlDisplay();
        exit(0);
    }
}

int quitFilter(void *userdata, SDL_Event *event)
{
    (void)userdata;
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_CLOSE)
    {
        if (auto window = findSdlDisplay(event->window.windowID))
        {
            SdlDisplays.remove(window);
        }
    }

    if (event->type == SDL_QUIT)
    {
        sdlQuitQry = true;
    }

    return 1;
}

static bool sdlInitialized = false;
void sdlInit()
{
    if (!sdlInitialized)
    {
        /*Initialize the SDL*/
        SDL_Init(SDL_INIT_VIDEO);
        SDL_SetEventFilter(quitFilter, nullptr);
        SDL_StartTextInput();
        lv_timer_create(sdlEventHandler, 10, nullptr);
        sdlInitialized = true;
    }
}

void sdlAddDisplay(lv_disp_drv_t *disp, std::function<void(void)> removeCb)
{
    auto SdlDisplay = std::make_shared<SdlDrvCfg>(disp, std::move(removeCb));
    SdlDisplays.push_back(SdlDisplay);
    if (SdlDisplays.size() > 1)
    {
        int x, y;
        SDL_GetWindowPosition((*++SdlDisplays.rbegin())->windowDrvCfg.window, &x, &y);
        SDL_SetWindowPosition(SdlDisplay->windowDrvCfg.window, x + 20, y + 20);
    }
}

void sdlSetMouse(lv_disp_drv_t *disp, uint32_t id)
{
    auto SdlDisplay = findSdlDisplay(disp);
    SdlDisplay->mouseDrvCfg.reset(new SdlDrvCfg::MouseDrvCfg(id));
}

void sdlSetWheel(lv_disp_drv_t *disp, uint32_t id)
{
    auto SdlDisplay = findSdlDisplay(disp);
    SdlDisplay->wheelDrvCfg.reset(new SdlDrvCfg::WheelDrvCfg(id));
}

void sdlSetKeyboard(lv_disp_drv_t *disp, uint32_t id)
{
    auto SdlDisplay = findSdlDisplay(disp);
    SdlDisplay->keyboardDrvCfg.reset(new SdlDrvCfg::KeyboardDrvCfg(id));
}

std::shared_ptr<SdlDrvCfg> findSdlDisplay(lv_disp_drv_t *drv)
{
    for (auto SdlDisplay : SdlDisplays)
    {
        if (SdlDisplay->windowDrvCfg.dispDrv == drv)
        {
            return SdlDisplay;
        }
    }
    return nullptr;
}

std::shared_ptr<SdlDrvCfg> findSdlDisplay(uint32_t id)
{
    for (auto SdlDisplay : SdlDisplays)
    {
        if (SdlDisplay->windowDrvCfg.displayId == id)
        {
            return SdlDisplay;
        }
    }
    return nullptr;
}

static void sdlRefrSdlDisplay()
{
    for (const auto &SdlDisplay : SdlDisplays)
    {
        /*Refresh handling*/
        if (SdlDisplay->windowDrvCfg.sdlRefrQry)
        {
            SdlDisplay->windowDrvCfg.sdlRefrQry = false;
            SdlDisplay->windowDrvCfg.update();
        }
    }
}

/**
 * Flush a buffer to the marked area
 * @param disp_drv pointer to driver where this function belongs
 * @param area an area where to copy `color_p`
 * @param color_p an array of pixels to copy to the `area` part of the screen
 */
void sdlDisplayFlush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    std::shared_ptr<SdlDrvCfg> SdlDisplay;
    if (SdlDisplays.size() == 1)
    {
        SdlDisplay = SdlDisplays.back();
    }
    else
    {
        SdlDisplay = findSdlDisplay(disp_drv);
    }
    lv_coord_t hres = disp_drv->physical_hor_res == -1 ? disp_drv->hor_res : disp_drv->physical_hor_res;
    lv_coord_t vres = disp_drv->physical_ver_res == -1 ? disp_drv->ver_res : disp_drv->physical_ver_res;

    /*Return if the area is out the screen*/
    if (area->x2 < 0 || area->y2 < 0 || area->x1 > hres - 1 || area->y1 > vres - 1)
    {
        lv_disp_flush_ready(disp_drv);
        return;
    }

#if SDL_DOUBLE_BUFFERED
    SdlDisplay.tftFrameBufferAct = (uint32_t *)color_p;
#else                                            /*SDL_DOUBLE_BUFFERED*/

    int32_t y;
#if LV_COLOR_DEPTH != 24 && LV_COLOR_DEPTH != 32 /*32 is valid but support 24 for backward compatibility too*/
    int32_t x;
    for (y = area->y1; y <= area->y2 && y < vres; y++)
    {
        for (x = area->x1; x <= area->x2; x++)
        {
            SdlDisplay.tft_fb[y * hres + x] = lv_color_to32(*color_p);
            color_p++;
        }
    }
#else
    uint32_t w = lv_area_get_width(area);
    for (y = area->y1; y <= area->y2 && y < vres; y++)
    {
        memcpy(&SdlDisplay->windowDrvCfg.tftFrameBuffer[y * hres + area->x1], color_p, w * sizeof(lv_color_t));
        color_p += w;
    }
#endif
#endif /*SDL_DOUBLE_BUFFERED*/

    SdlDisplay->windowDrvCfg.sdlRefrQry = true;

    /* TYPICALLY YOU DO NOT NEED THIS
     * If it was the last part to refresh update the texture of the window.*/
    if (lv_disp_flush_is_last(disp_drv))
    {
        sdlRefrSdlDisplay();
    }

    /*IMPORTANT! It must be called to tell the system the flush is ready*/
    lv_disp_flush_ready(disp_drv);
}

void SdlDrvCfg::DisplayDrvCfg::create(lv_disp_drv_t *disp)
{
    dispDrv = disp;
    int flag = 0;
#if SDL_FULLSCREEN
    flag |= SDL_WINDOW_FULLSCREEN;
#endif

    window =
        SDL_CreateWindow("TFT Simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, disp->hor_res * SDL_ZOOM,
                         disp->ver_res * SDL_ZOOM, flag); /*last param. SDL_WINDOW_BORDERLESS to hide borders*/
    displayId = SDL_GetWindowID(window);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    texture =
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, disp->hor_res, disp->ver_res);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    /*Initialize the frame buffer to gray (77 is an empirical value) */
#if SDL_DOUBLE_BUFFERED
    SDL_UpdateTexture(texture, NULL, tftFrameBufferAct, disp->hor_res * sizeof(uint32_t));
#else
    tftFrameBuffer = (uint32_t *)malloc(sizeof(uint32_t) * disp->hor_res * disp->ver_res);
    memset(tftFrameBuffer, 0x44, disp->hor_res * disp->ver_res * sizeof(uint32_t));
#endif

    sdlRefrQry = true;
}

void SdlDrvCfg::DisplayDrvCfg::update() const
{
#if SDL_DOUBLE_BUFFERED == 0
    SDL_UpdateTexture(texture, nullptr, tftFrameBuffer, dispDrv->hor_res * sizeof(uint32_t));
#else
    if (tftFrameBufferAct == NULL)
        return;
    SDL_UpdateTexture(texture, nullptr, tftFrameBufferAct, disp->hor_res * sizeof(uint32_t));
#endif
    SDL_RenderClear(renderer);
    lv_disp_t *d = _lv_refr_get_disp_refreshing();
    if (d->driver->screen_transp)
    {
        SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0xff);
        SDL_Rect r;
        r.x = 0;
        r.y = 0;
        r.w = dispDrv->hor_res;
        r.h = dispDrv->ver_res;
        SDL_RenderDrawRect(renderer, &r);
    }

    /*Update the renderer with the texture containing the rendered image*/
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void addSdlPeripherals(unsigned short displayWidth, unsigned short displayHeight, bool addMouse, bool addWheel,
                       bool addKeyboard)
{
    sdlInit();

    auto display = softeq::lvglplus::LvglPlus::addDisplay(displayWidth, displayHeight, sdlDisplayFlush);
    auto displayDriver = display->lvDisplay()->driver;
    auto disp = display->lvDisplay();
    sdlAddDisplay(displayDriver, [disp]() { softeq::lvglplus::LvglPlus::removeDisplay(disp); });
    if (addMouse)
    {
        sdlSetMouse(displayDriver);
        softeq::lvglplus::LvglPlus::addMouse(sdlMouseRead, display);
    }
    if (addWheel)
    {
        sdlSetWheel(displayDriver);
        softeq::lvglplus::LvglPlus::addEncoder(sdlMousewheelRead, display);
    }
    if (addKeyboard)
    {
        sdlSetKeyboard(displayDriver);
        softeq::lvglplus::LvglPlus::addKeyboard(sdlKeyboardRead, display);
    }
}

#endif // USE_SDL
