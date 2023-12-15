#ifndef LVGL_CUSTOM_THEME_HH
#define LVGL_CUSTOM_THEME_HH

#include "lvgl.h"
#include "widget.hh"
#include "widgets/properties/padding.hh"
#include <memory>

namespace softeq
{
namespace lvglplus
{
using Offset = Point;

template <typename T>
class InitCtrl_t
{
public:
    explicit InitCtrl_t(const T &value)
        : _initialized(true)
        , _value(value){};
    InitCtrl_t()
        : _initialized(false){};

    void set(const T &value)
    {
        _initialized = true;
        _value = value;
    }

    bool isSet() const
    {
        return _initialized;
    }

    const T &get() const
    {
        return _value;
    }

private:
    bool _initialized = false;
    T _value;
};

struct ThemeParams
{
    InitCtrl_t<unsigned long> backgroundColor;
    InitCtrl_t<unsigned long> mainColor;
    InitCtrl_t<unsigned long> borderColor;
    InitCtrl_t<unsigned long> secondaryColor;
    InitCtrl_t<unsigned long> buttonBackgroundColor;
    InitCtrl_t<unsigned long> widgetBackgroundColor;
    InitCtrl_t<unsigned long> inactiveColor;
    InitCtrl_t<unsigned long> trackColor;
    InitCtrl_t<short> trackOpacity;
    InitCtrl_t<unsigned long> indicatorColor;
    InitCtrl_t<unsigned long> knobColor;
    InitCtrl_t<unsigned long> switchKnobColor;
    InitCtrl_t<unsigned long> textColor;
    InitCtrl_t<unsigned long> layoutColor;
    InitCtrl_t<unsigned long> shadowColor;
    InitCtrl_t<short> shadowWidth;
    InitCtrl_t<short> shadowOpacity;
    InitCtrl_t<Offset> shadowOffset;
    InitCtrl_t<short> layoutOpacity;
    InitCtrl_t<short> textSize;
    InitCtrl_t<short> borderWidth;
    InitCtrl_t<short> layoutBorderWidth;
    InitCtrl_t<Padding> layoutPadding;
};

class ThemeBuilder
{
private:
    ThemeParams _params;

public:
    const ThemeParams &params() const
    {
        return _params;
    }

    ThemeBuilder &backgroundColor(unsigned long color)
    {
        _params.backgroundColor.set(color);
        return *this;
    }

    ThemeBuilder &buttonBackgroundColor(unsigned long color)
    {
        _params.buttonBackgroundColor.set(color);
        return *this;
    }

    ThemeBuilder &widgetBackgroundColor(unsigned long color)
    {
        _params.widgetBackgroundColor.set(color);
        return *this;
    }

    ThemeBuilder &mainColor(unsigned long color)
    {
        _params.mainColor.set(color);
        return *this;
    }

    ThemeBuilder &borderColor(unsigned long color)
    {
        _params.borderColor.set(color);
        return *this;
    }

    ThemeBuilder &layoutColor(unsigned long color)
    {
        _params.layoutColor.set(color);
        return *this;
    }

    ThemeBuilder &secondaryColor(unsigned long color)
    {
        _params.secondaryColor.set(color);
        return *this;
    }

    ThemeBuilder &inactiveColor(unsigned long color)
    {
        _params.inactiveColor.set(color);
        return *this;
    }

    ThemeBuilder &trackColor(unsigned long color)
    {
        _params.trackColor.set(color);
        if (!_params.trackOpacity.isSet())
        {
            _params.trackOpacity.set(255);
        }
        return *this;
    }

    ThemeBuilder &trackOpacity(short opacity)
    {
        _params.trackOpacity.set(opacity);
        return *this;
    }

    ThemeBuilder &indicatorColor(unsigned long color)
    {
        _params.indicatorColor.set(color);
        return *this;
    }

    ThemeBuilder &knobColor(unsigned long color)
    {
        _params.knobColor.set(color);
        return *this;
    }

    ThemeBuilder &switchKnobColor(unsigned long color)
    {
        _params.switchKnobColor.set(color);
        return *this;
    }

    ThemeBuilder &textColor(unsigned long color)
    {
        _params.textColor.set(color);
        return *this;
    }

    ThemeBuilder &textSize(short value)
    {
        _params.textSize.set(value);
        return *this;
    }

    ThemeBuilder &layoutOpacity(short value)
    {
        _params.layoutOpacity.set(value);
        return *this;
    }

    ThemeBuilder &borderWidth(short value)
    {
        _params.borderWidth.set(value);
        return *this;
    }

    ThemeBuilder &layoutBorderWidth(short value)
    {
        _params.layoutBorderWidth.set(value);
        return *this;
    }

    ThemeBuilder &shadowColor(unsigned long color)
    {
        _params.shadowColor.set(color);
        return *this;
    }

    ThemeBuilder &shadowWidth(short value)
    {
        _params.shadowWidth.set(value);
        return *this;
    }

    ThemeBuilder &shadowOpacity(short value)
    {
        _params.shadowOpacity.set(value);
        return *this;
    }

    ThemeBuilder &shadowOffset(Offset offset)
    {
        _params.shadowOffset.set(offset);
        return *this;
    }

    ThemeBuilder &layoutPadding(Padding padding)
    {
        _params.layoutPadding.set(padding);
        return *this;
    }
};

class Theme
{
public:
    explicit Theme(const ThemeBuilder &paramsBuilder)
    {
        _params = paramsBuilder.params();
        if (!_params.textColor.isSet())
        {
            _params.textColor.set(lv_palette_darken(LV_PALETTE_GREY, 4).full);
        }
        if (!_params.backgroundColor.isSet())
        {
            _params.backgroundColor.set(lv_palette_lighten(LV_PALETTE_GREY, 3).full);
        }
        if (!_params.mainColor.isSet())
        {
            _params.mainColor.set(lv_color_white().full);
        }
        if (!_params.secondaryColor.isSet())
        {
            _params.secondaryColor.set(lv_color_white().full);
        }
        if (!_params.inactiveColor.isSet())
        {
            _params.inactiveColor.set(lv_palette_lighten(LV_PALETTE_GREY, 2).full);
        }
        if (!_params.knobColor.isSet())
        {
            _params.knobColor.set(_params.mainColor.get());
        }
        if (!_params.switchKnobColor.isSet())
        {
            _params.switchKnobColor.set(lv_color_white().full);
        }
        if (!_params.borderColor.isSet())
        {
            _params.borderColor.set(_params.mainColor.get());
        }
        if (!_params.buttonBackgroundColor.isSet())
        {
            _params.buttonBackgroundColor.set(_params.mainColor.get());
        }
        if (!_params.widgetBackgroundColor.isSet())
        {
            _params.widgetBackgroundColor.set(_params.inactiveColor.get());
        }
        if (!_params.trackColor.isSet())
        {
            _params.trackColor.set(_params.mainColor.get());
        }
        if (!_params.trackOpacity.isSet())
        {
            _params.trackOpacity.set(LV_OPA_20);
        }
        if (!_params.borderWidth.isSet())
        {
            _params.borderWidth.set(1);
        }
        if (!_params.shadowColor.isSet())
        {
            _params.shadowColor.set(lv_palette_main(LV_PALETTE_GREY).full);
        }
        if (!_params.shadowWidth.isSet())
        {
            _params.shadowWidth.set(LV_DPX(3));
        }
        if (!_params.shadowOpacity.isSet())
        {
            _params.shadowOpacity.set(LV_OPA_50);
        }
        if (!_params.shadowOffset.isSet())
        {
            _params.shadowOffset.set(Offset(0, lv_disp_dpx(_disp, LV_DPX(4))));
        }
        if (!_params.layoutPadding.isSet())
        {
            _params.layoutPadding.set(Padding(5));
        }
        if (!_params.layoutBorderWidth.isSet())
        {
            _params.layoutBorderWidth.set(0);
        }
        if (!_params.layoutColor.isSet())
        {
            _params.layoutColor.set(_params.backgroundColor.get());
        }
        lvInit();
    }

    ~Theme()
    {
        remove();
    }

    void setBackgroundColor(unsigned long color)
    {
        _params.backgroundColor.set(color);
    }

    void setButtonBackgroundColor(unsigned long color)
    {
        _params.buttonBackgroundColor.set(color);
    }

    void setMainColor(unsigned long color)
    {
        _params.mainColor.set(color);
    }

    void setBorderColor(unsigned long color)
    {
        _params.borderColor.set(color);
    }

    void setLayoutColor(unsigned long color)
    {
        _params.layoutColor.set(color);
    }

    void setSecondaryColor(unsigned long color)
    {
        _params.secondaryColor.set(color);
    }

    void setInactiveColor(unsigned long color)
    {
        _params.inactiveColor.set(color);
    }

    void setTrackColor(unsigned long color)
    {
        _params.trackColor.set(color);
        if (!_params.trackOpacity.isSet())
        {
            _params.trackOpacity.set(255);
        }
    }

    void setTrackOpacity(short opacity)
    {
        _params.trackOpacity.set(opacity);
    }

    void setIndicatorColor(unsigned long color)
    {
        _params.indicatorColor.set(color);
    }

    void setKnobColor(unsigned long color)
    {
        _params.knobColor.set(color);
    }

    void setSwitchKnobColor(unsigned long color)
    {
        _params.switchKnobColor.set(color);
    }

    void setTextColor(unsigned long color)
    {
        _params.textColor.set(color);
    }

    void setTextSize(short value)
    {
        _params.textSize.set(value);
    }

    void setLayoutOpacity(short value)
    {
        _params.layoutOpacity.set(value);
    }

    void setBorderWidth(short value)
    {
        _params.borderWidth.set(value);
    }

    void setlayoutBorderWidth(short value)
    {
        _params.layoutBorderWidth.set(value);
    }

    void setShadowColor(unsigned long color)
    {
        _params.shadowColor.set(color);
    }

    void setShadowWidth(short value)
    {
        _params.shadowWidth.set(value);
    }

    void setShadowOpacity(short value)
    {
        _params.shadowOpacity.set(value);
    }

    void setShadowOffset(Offset offset)
    {
        _params.shadowOffset.set(offset);
    }

    void setLayoutPadding(Padding padding)
    {
        _params.layoutPadding.set(padding);
    }

    void apply()
    {
        applyThemeToChildren(lv_disp_get_scr_act(_disp));
    }
    _lv_theme_t *defaultTheme;
    void remove()
    {
        if (_applied)
        {
            _disp->theme = _disp->theme->parent;
            removeThemeFromChildren(lv_disp_get_scr_act(_disp));
        }
    }

    static void themeApplyCallback(lv_theme_t *th, lv_obj_t *obj)
    {
        reinterpret_cast<Theme *>(th->user_data)->themeApply(obj);
    }

    void setDisp(lv_disp_t *disp)
    {
        _disp = disp;
        theme.disp = _disp;
        theme.parent = _disp->theme;
        theme.user_data = this;
        theme.apply_cb = themeApplyCallback;
        lv_disp_set_theme(_disp, &theme);
    }

private:
    void applyThemeToChildren(lv_obj_t *parent);
    void removeThemeFromChildren(lv_obj_t *parent);
    static const short _transitionTime = LV_THEME_DEFAULT_TRANSITION_TIME;
    static const short _outlineWidth = 0;

    struct StyleContainer
    {
        lv_style_t scr;
        lv_style_t scrollbar;
        lv_style_t scrollbarScrolled;
        lv_style_t card;
        lv_style_t btn;
        lv_style_t layout;

        /*Utility*/
        lv_style_t trackColor;
        lv_style_t bgColorPrimary;
        lv_style_t bgColorPrimaryMuted;
        lv_style_t bgColorSecondary;
        lv_style_t bgColorSecondaryMuted;
        lv_style_t bgColorGrey;
        lv_style_t bgColorWhite;
        lv_style_t pressed;
        lv_style_t disabled;
        lv_style_t padZero;
        lv_style_t padTiny;
        lv_style_t padSmall;
        lv_style_t padNormal;
        lv_style_t padGap;
        lv_style_t lineSpaceLarge;
        lv_style_t textAlignCenter;
        lv_style_t outlinePrimary;
        lv_style_t outlineSecondary;
        lv_style_t circle;
        lv_style_t noRadius;
        lv_style_t clipCorner;
#if LV_THEME_DEFAULT_GROW
        lv_style_t grow;
#endif
        lv_style_t transitionDelayed;
        lv_style_t transitionNormal;
        lv_style_t anim;
        lv_style_t animFast;

        /*Parts*/
        lv_style_t knob;
        lv_style_t indic;

#if LV_USE_ARC
        lv_style_t arcIndic;
        lv_style_t arcTrack;
        lv_style_t arcIndicPrimary;
#endif

#if LV_USE_CHART
        lv_style_t chartSeries, chartIndic, chartTicks, chartBg;
#endif

#if LV_USE_DROPDOWN
        lv_style_t dropdownList;
#endif

#if LV_USE_CHECKBOX
        lv_style_t cbMarker, cbMarkerChecked;
#endif

#if LV_USE_SWITCH
        lv_style_t switchKnob;
#endif

#if LV_USE_LINE
        lv_style_t line;
#endif

#if LV_USE_TABLE
        lv_style_t tableCell;
#endif

#if LV_USE_METER
        lv_style_t meterMarker, meterIndic;
#endif

#if LV_USE_TEXTAREA
        lv_style_t taCursor, taPlaceholder;
#endif

#if LV_USE_CALENDAR
        lv_style_t calendarBtnmBg, calendarBtnmDay, calendarHeader;
#endif

#if LV_USE_COLORWHEEL
        lv_style_t colorwheelMain;
#endif

#if LV_USE_MENU
        lv_style_t menuBg, menuCont, menuSidebarCont, menuMainCont, menuPage, menuHeaderCont, menuHeaderBtn,
            menuSection, menuPressed, menuSeparator;
#endif

#if LV_USE_MSGBOX
        lv_style_t msgboxBg, msgboxBtnBg, msgboxBackdropBg;
#endif

#if LV_USE_KEYBOARD
        lv_style_t keyboardBtnBg;
#endif

#if LV_USE_LIST
        lv_style_t listBg, listBtn, listItemGrow, listLabel;
#endif

#if LV_USE_TABVIEW
        lv_style_t tabBgFocus, tabBtn;
#endif
#if LV_USE_LED
        lv_style_t led;
#endif
    };

    enum class DispSize
    {
        dispSmall = 3,
        dispMedium = 2,
        dispLarge = 1,
    };

    std::shared_ptr<StyleContainer> _styles;
    DispSize _dispSize = DispSize::dispMedium;

    ThemeParams _params;

    lv_theme_t theme;
    lv_disp_t *_disp = nullptr;
    bool _initialized = false;
    bool _applied = false;
    bool _removing = false;

    const lv_font_t *_fontSmall = LV_FONT_DEFAULT;
    const lv_font_t *_fontNormal = LV_FONT_DEFAULT;
    const lv_font_t *_fontLarge = LV_FONT_DEFAULT;

    void lvInit();

    void styleInit();
    lv_coord_t radiusDefault();
    lv_coord_t padTiny();
    lv_coord_t padSmall();
    lv_coord_t padDef();
    void themeApply(lv_obj_t *obj);
    void styleInitReset(lv_style_t *style) const;
    void replaceStyle(lv_obj_t *obj, lv_style_t *style, lv_style_selector_t selector) const;
};

} // namespace lvglplus
} // namespace softeq

#endif /*LVGL_CUSTOM_THEME_HH*/
