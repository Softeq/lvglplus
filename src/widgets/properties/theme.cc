#include "widgets/properties/theme.hh"
#include "lvgl.h"
#include "lvgl_plus.hh"

namespace softeq
{
namespace lvglplus
{

lv_coord_t Theme::padDef()
{
    return (_dispSize == DispSize::dispLarge    ? lv_disp_dpx(_disp, 24)
            : _dispSize == DispSize::dispMedium ? lv_disp_dpx(_disp, 20)
                                                : lv_disp_dpx(_disp, 16));
}
lv_coord_t Theme::padSmall()
{
    return (_dispSize == DispSize::dispLarge    ? lv_disp_dpx(_disp, 14)
            : _dispSize == DispSize::dispMedium ? lv_disp_dpx(_disp, 12)
                                                : lv_disp_dpx(_disp, 10));
}
lv_coord_t Theme::padTiny()
{
    return (_dispSize == DispSize::dispLarge    ? lv_disp_dpx(_disp, 8)
            : _dispSize == DispSize::dispMedium ? lv_disp_dpx(_disp, 6)
                                                : lv_disp_dpx(_disp, 2));
}
lv_coord_t Theme::radiusDefault()
{
    return (_dispSize == DispSize::dispLarge ? lv_disp_dpx(_disp, 12) : lv_disp_dpx(_disp, 8));
}

static lv_color_t darkColorFilterCb(const lv_color_filter_dsc_t *f, lv_color_t c, lv_opa_t opa)
{
    LV_UNUSED(f);
    return lv_color_darken(c, opa);
}

static lv_color_t greyFilterCb(const lv_color_filter_dsc_t *f, lv_color_t color, lv_opa_t opa)
{
    LV_UNUSED(f);
    return lv_color_mix(lv_palette_lighten(LV_PALETTE_GREY, 2), color, opa);
}

void Theme::styleInit()
{
    UniqueLock locker(LvglPlus::lvglMutex());
    static const lv_style_prop_t trans_props[] = {
        LV_STYLE_BG_OPA,           LV_STYLE_BG_COLOR,         LV_STYLE_TRANSFORM_WIDTH, LV_STYLE_TRANSFORM_HEIGHT,
        LV_STYLE_TRANSLATE_Y,      LV_STYLE_TRANSLATE_X,      LV_STYLE_TRANSFORM_ZOOM,  LV_STYLE_TRANSFORM_ANGLE,
        LV_STYLE_COLOR_FILTER_OPA, LV_STYLE_COLOR_FILTER_DSC, (lv_style_prop_t)0};

    styleInitReset(&_styles->transitionDelayed);
    styleInitReset(&_styles->transitionNormal);

    styleInitReset(&_styles->scrollbar);
    lv_color_t sb_color = lv_palette_main(LV_PALETTE_GREY);
    lv_style_set_bg_color(&_styles->scrollbar, sb_color);

    lv_style_set_radius(&_styles->scrollbar, LV_RADIUS_CIRCLE);
    lv_style_set_pad_all(&_styles->scrollbar, lv_disp_dpx(_disp, 7));
    lv_style_set_width(&_styles->scrollbar, lv_disp_dpx(_disp, 5));
    lv_style_set_bg_opa(&_styles->scrollbar, LV_OPA_40);
    if (_transitionTime)
    {
        static lv_style_transition_dsc_t trans_delayed;
        lv_style_transition_dsc_init(&trans_delayed, trans_props, lv_anim_path_linear, _transitionTime, 70, nullptr);

        static lv_style_transition_dsc_t trans_normal;
        lv_style_transition_dsc_init(&trans_normal, trans_props, lv_anim_path_linear, _transitionTime, 0, nullptr);

        lv_style_set_transition(&_styles->transitionDelayed, &trans_delayed); /*Go back to default state with delay*/

        lv_style_set_transition(&_styles->transitionNormal, &trans_normal); /*Go back to default state with delay*/
        lv_style_set_transition(&_styles->scrollbar, &trans_normal);
    }

    styleInitReset(&_styles->scrollbarScrolled);
    lv_style_set_bg_opa(&_styles->scrollbarScrolled, LV_OPA_COVER);

    styleInitReset(&_styles->scr);
    lv_style_set_bg_opa(&_styles->scr, LV_OPA_COVER);
    lv_style_set_bg_color(&_styles->scr, lv_color_hex(_params.backgroundColor.get()));
    lv_style_set_text_color(&_styles->scr, lv_color_hex(_params.textColor.get()));
    lv_style_set_radius(&_styles->scr, 0);
    lv_style_set_pad_row(&_styles->scr, padSmall());
    lv_style_set_pad_column(&_styles->scr, padSmall());

    styleInitReset(&_styles->layout);
    lv_style_set_radius(&_styles->layout, radiusDefault());
    lv_style_set_pad_top(&_styles->layout, _params.layoutPadding.get().getTop());
    lv_style_set_pad_bottom(&_styles->layout, _params.layoutPadding.get().getBottom());
    lv_style_set_pad_left(&_styles->layout, _params.layoutPadding.get().getLeft());
    lv_style_set_pad_right(&_styles->layout, _params.layoutPadding.get().getRight());
    lv_style_set_pad_row(&_styles->layout, _params.layoutPadding.get().getRow());
    lv_style_set_pad_column(&_styles->layout, _params.layoutPadding.get().getColumn());
    lv_style_set_border_width(&_styles->layout, _params.layoutBorderWidth.get());
    lv_style_set_bg_opa(&_styles->layout, static_cast<uint8_t>(_params.layoutOpacity.get()));
    lv_style_set_bg_color(&_styles->layout, lv_color_hex(_params.layoutColor.get()));

    styleInitReset(&_styles->card);
    lv_style_set_radius(&_styles->card, radiusDefault());
    lv_style_set_bg_color(&_styles->card, lv_color_hex(_params.widgetBackgroundColor.get()));
    lv_style_set_border_color(&_styles->card, lv_color_hex(_params.borderColor.get()));
    lv_style_set_border_width(&_styles->card, _params.borderWidth.get());
    lv_style_set_border_post(&_styles->card, true);
    lv_style_set_text_color(&_styles->card, lv_color_hex(_params.textColor.get()));
    lv_style_set_pad_all(&_styles->card, padDef());
    lv_style_set_pad_row(&_styles->card, padSmall());
    lv_style_set_pad_column(&_styles->card, padSmall());
    lv_style_set_line_color(&_styles->card, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_line_width(&_styles->card, lv_disp_dpx(_disp, 1));

    styleInitReset(&_styles->outlinePrimary);
    lv_style_set_outline_color(&_styles->outlinePrimary, lv_color_hex(_params.mainColor.get()));
    lv_style_set_outline_width(&_styles->outlinePrimary, _outlineWidth);
    lv_style_set_outline_pad(&_styles->outlinePrimary, _outlineWidth);
    lv_style_set_outline_opa(&_styles->outlinePrimary, LV_OPA_50);

    styleInitReset(&_styles->outlineSecondary);
    lv_style_set_outline_color(&_styles->outlineSecondary, lv_color_hex(_params.secondaryColor.get()));
    lv_style_set_outline_width(&_styles->outlineSecondary, _outlineWidth);
    lv_style_set_outline_opa(&_styles->outlineSecondary, LV_OPA_50);

    styleInitReset(&_styles->btn);
    lv_style_set_radius(&_styles->btn, (_dispSize == DispSize::dispLarge    ? lv_disp_dpx(_disp, 16)
                                        : _dispSize == DispSize::dispMedium ? lv_disp_dpx(_disp, 12)
                                                                            : lv_disp_dpx(_disp, 8)));
    lv_style_set_bg_opa(&_styles->btn, LV_OPA_COVER);
    lv_style_set_bg_color(&_styles->btn, lv_color_hex(_params.buttonBackgroundColor.get()));

    lv_style_set_shadow_color(&_styles->btn, lv_color_hex(_params.shadowColor.get()));
    lv_style_set_shadow_width(&_styles->btn, _params.shadowWidth.get());
    lv_style_set_shadow_opa(&_styles->btn, _params.shadowOpacity.get());
    lv_style_set_shadow_ofs_x(&_styles->btn, _params.shadowOffset.get().x);
    lv_style_set_shadow_ofs_y(&_styles->btn, _params.shadowOffset.get().y);

    lv_style_set_text_color(&_styles->btn, lv_color_hex(_params.textColor.get()));
    lv_style_set_pad_hor(&_styles->btn, padDef());
    lv_style_set_pad_ver(&_styles->btn, padSmall());
    lv_style_set_pad_column(&_styles->btn, lv_disp_dpx(_disp, 5));
    lv_style_set_pad_row(&_styles->btn, lv_disp_dpx(_disp, 5));

    static lv_color_filter_dsc_t dark_filter;
    lv_color_filter_dsc_init(&dark_filter, darkColorFilterCb);

    static lv_color_filter_dsc_t grey_filter;
    lv_color_filter_dsc_init(&grey_filter, greyFilterCb);

    styleInitReset(&_styles->pressed);
    lv_style_set_color_filter_dsc(&_styles->pressed, &dark_filter);
    lv_style_set_color_filter_opa(&_styles->pressed, 35);

    styleInitReset(&_styles->disabled);
    lv_style_set_color_filter_dsc(&_styles->disabled, &grey_filter);
    lv_style_set_color_filter_opa(&_styles->disabled, LV_OPA_50);

    styleInitReset(&_styles->clipCorner);
    lv_style_set_clip_corner(&_styles->clipCorner, true);
    lv_style_set_border_post(&_styles->clipCorner, true);

    styleInitReset(&_styles->padNormal);
    lv_style_set_pad_all(&_styles->padNormal, padDef());
    lv_style_set_pad_row(&_styles->padNormal, padDef());
    lv_style_set_pad_column(&_styles->padNormal, padDef());

    styleInitReset(&_styles->padSmall);
    lv_style_set_pad_all(&_styles->padSmall, padSmall());
    lv_style_set_pad_gap(&_styles->padSmall, padSmall());

    styleInitReset(&_styles->padGap);
    lv_style_set_pad_row(&_styles->padGap, lv_disp_dpx(_disp, 10));
    lv_style_set_pad_column(&_styles->padGap, lv_disp_dpx(_disp, 10));

    styleInitReset(&_styles->lineSpaceLarge);
    lv_style_set_text_line_space(&_styles->lineSpaceLarge, lv_disp_dpx(_disp, 20));

    styleInitReset(&_styles->textAlignCenter);
    lv_style_set_text_align(&_styles->textAlignCenter, LV_TEXT_ALIGN_CENTER);

    styleInitReset(&_styles->padZero);
    lv_style_set_pad_all(&_styles->padZero, 0);
    lv_style_set_pad_row(&_styles->padZero, 0);
    lv_style_set_pad_column(&_styles->padZero, 0);

    styleInitReset(&_styles->padTiny);
    lv_style_set_pad_all(&_styles->padTiny, padTiny());
    lv_style_set_pad_row(&_styles->padTiny, padTiny());
    lv_style_set_pad_column(&_styles->padTiny, padTiny());

    styleInitReset(&_styles->bgColorPrimary);
    lv_style_set_bg_color(&_styles->bgColorPrimary, lv_color_hex(_params.mainColor.get()));
    lv_style_set_text_color(&_styles->bgColorPrimary, lv_color_white());
    lv_style_set_bg_opa(&_styles->bgColorPrimary, LV_OPA_COVER);

    styleInitReset(&_styles->trackColor);
    lv_style_set_bg_color(&_styles->trackColor, lv_color_hex(_params.trackColor.get()));
    lv_style_set_bg_opa(&_styles->trackColor, static_cast<uint8_t>(_params.trackOpacity.get()));

    styleInitReset(&_styles->bgColorPrimaryMuted);
    lv_style_set_bg_color(&_styles->bgColorPrimaryMuted, lv_color_hex(_params.mainColor.get()));
    lv_style_set_text_color(&_styles->bgColorPrimaryMuted, lv_color_hex(_params.mainColor.get()));
    lv_style_set_bg_opa(&_styles->bgColorPrimaryMuted, LV_OPA_20);

    styleInitReset(&_styles->bgColorSecondary);
    lv_style_set_bg_color(&_styles->bgColorSecondary, lv_color_hex(_params.secondaryColor.get()));
    lv_style_set_text_color(&_styles->bgColorSecondary, lv_color_white());
    lv_style_set_bg_opa(&_styles->bgColorSecondary, LV_OPA_COVER);

    styleInitReset(&_styles->bgColorSecondaryMuted);
    lv_style_set_bg_color(&_styles->bgColorSecondaryMuted, lv_color_hex(_params.secondaryColor.get()));
    lv_style_set_text_color(&_styles->bgColorSecondaryMuted, lv_color_hex(_params.secondaryColor.get()));
    lv_style_set_bg_opa(&_styles->bgColorSecondaryMuted, LV_OPA_20);

    styleInitReset(&_styles->bgColorGrey);
    lv_style_set_bg_color(&_styles->bgColorGrey, lv_color_hex(_params.inactiveColor.get()));
    lv_style_set_bg_opa(&_styles->bgColorGrey, LV_OPA_COVER);
    lv_style_set_text_color(&_styles->bgColorGrey, lv_color_hex(_params.textColor.get()));

    styleInitReset(&_styles->bgColorWhite);
    lv_style_set_bg_color(&_styles->bgColorWhite, lv_color_hex(_params.inactiveColor.get()));
    lv_style_set_bg_opa(&_styles->bgColorWhite, LV_OPA_COVER);
    lv_style_set_text_color(&_styles->bgColorWhite, lv_color_hex(_params.textColor.get()));

    styleInitReset(&_styles->circle);
    lv_style_set_radius(&_styles->circle, LV_RADIUS_CIRCLE);

    styleInitReset(&_styles->noRadius);
    lv_style_set_radius(&_styles->noRadius, 0);

#if LV_THEME_DEFAULT_GROW
    styleInitReset(&_styles->grow);
    lv_style_set_transform_width(&_styles->grow, lv_disp_dpx(_disp, 3));
    lv_style_set_transform_height(&_styles->grow, lv_disp_dpx(_disp, 3));
#endif

    styleInitReset(&_styles->knob);
    lv_style_set_bg_color(&_styles->knob, lv_color_hex(_params.knobColor.get()));
    lv_style_set_bg_opa(&_styles->knob, LV_OPA_COVER);
    lv_style_set_pad_all(&_styles->knob, lv_disp_dpx(_disp, 6));
    lv_style_set_radius(&_styles->knob, LV_RADIUS_CIRCLE);

    styleInitReset(&_styles->anim);
    lv_style_set_anim_time(&_styles->anim, 200);

    styleInitReset(&_styles->animFast);
    lv_style_set_anim_time(&_styles->animFast, 120);

#if LV_USE_ARC
    styleInitReset(&_styles->arcIndic);
    lv_style_set_arc_color(&_styles->arcIndic, lv_color_hex(_params.mainColor.get()));
    lv_style_set_arc_width(&_styles->arcIndic, lv_disp_dpx(_disp, 15));
    lv_style_set_arc_rounded(&_styles->arcIndic, true);

    styleInitReset(&_styles->arcTrack);
    lv_style_set_arc_color(&_styles->arcTrack, lv_color_hex(_params.trackColor.get()));
    lv_style_set_arc_opa(&_styles->arcTrack, _params.trackOpacity.get());
    lv_style_set_arc_width(&_styles->arcTrack, lv_disp_dpx(_disp, 15));
    lv_style_set_arc_rounded(&_styles->arcTrack, true);

    styleInitReset(&_styles->arcIndicPrimary);
    lv_style_set_arc_color(&_styles->arcIndicPrimary, lv_color_hex(_params.mainColor.get()));
#endif

#if LV_USE_DROPDOWN
    styleInitReset(&_styles->dropdownList);
    lv_style_set_max_height(&_styles->dropdownList, LV_DPI_DEF * 2);
#endif

#if LV_USE_CHECKBOX
    styleInitReset(&_styles->cbMarker);
    lv_style_set_pad_all(&_styles->cbMarker, lv_disp_dpx(_disp, 3));
    lv_style_set_border_width(&_styles->cbMarker, _params.borderWidth.get());
    lv_style_set_border_color(&_styles->cbMarker, lv_color_hex(_params.borderColor.get()));
    lv_style_set_bg_color(&_styles->cbMarker, lv_color_hex(_params.inactiveColor.get()));
    lv_style_set_bg_opa(&_styles->cbMarker, LV_OPA_COVER);
    lv_style_set_radius(&_styles->cbMarker, radiusDefault() / 2);

    styleInitReset(&_styles->cbMarkerChecked);
    lv_style_set_bg_img_src(&_styles->cbMarkerChecked, LV_SYMBOL_OK);
    lv_style_set_text_color(&_styles->cbMarkerChecked, lv_color_white());
    lv_style_set_text_font(&_styles->cbMarkerChecked, _fontSmall);
#endif

#if LV_USE_SWITCH
    styleInitReset(&_styles->switchKnob);
    lv_style_set_pad_all(&_styles->switchKnob, -lv_disp_dpx(_disp, 4));
    lv_style_set_bg_color(&_styles->switchKnob, lv_color_hex(_params.switchKnobColor.get()));
#endif

#if LV_USE_LINE
    styleInitReset(&_styles->line);
    lv_style_set_line_width(&_styles->line, 1);
    lv_style_set_line_color(&_styles->line, lv_color_hex(_params.textColor.get()));
#endif

#if LV_USE_CHART
    styleInitReset(&_styles->chartBg);
    lv_style_set_border_post(&_styles->chartBg, false);
    lv_style_set_pad_column(&_styles->chartBg, lv_disp_dpx(_disp, 10));
    lv_style_set_line_color(&_styles->chartBg, lv_color_hex(_params.inactiveColor.get()));

    styleInitReset(&_styles->chartSeries);
    lv_style_set_line_width(&_styles->chartSeries, lv_disp_dpx(_disp, 3));
    lv_style_set_radius(&_styles->chartSeries, lv_disp_dpx(_disp, 3));
    lv_style_set_size(&_styles->chartSeries, lv_disp_dpx(_disp, 8));
    lv_style_set_pad_column(&_styles->chartSeries, lv_disp_dpx(_disp, 2));

    styleInitReset(&_styles->chartIndic);
    lv_style_set_radius(&_styles->chartIndic, LV_RADIUS_CIRCLE);
    lv_style_set_size(&_styles->chartIndic, lv_disp_dpx(_disp, 8));
    lv_style_set_bg_color(&_styles->chartIndic, lv_color_hex(_params.mainColor.get()));
    lv_style_set_bg_opa(&_styles->chartIndic, LV_OPA_COVER);

    styleInitReset(&_styles->chartTicks);
    lv_style_set_line_width(&_styles->chartTicks, lv_disp_dpx(_disp, 1));
    lv_style_set_line_color(&_styles->chartTicks, lv_color_hex(_params.textColor.get()));
    lv_style_set_pad_all(&_styles->chartTicks, lv_disp_dpx(_disp, 2));
    lv_style_set_text_color(&_styles->chartTicks, lv_palette_main(LV_PALETTE_GREY));
#endif

#if LV_USE_MENU
    styleInitReset(&_styles->menuBg);
    lv_style_set_pad_all(&_styles->menuBg, 0);
    lv_style_set_pad_gap(&_styles->menuBg, 0);
    lv_style_set_radius(&_styles->menuBg, 0);
    lv_style_set_clip_corner(&_styles->menuBg, true);
    lv_style_set_border_side(&_styles->menuBg, LV_BORDER_SIDE_NONE);

    styleInitReset(&_styles->menuSection);
    lv_style_set_radius(&_styles->menuSection, radiusDefault());
    lv_style_set_clip_corner(&_styles->menuSection, true);
    lv_style_set_bg_opa(&_styles->menuSection, LV_OPA_COVER);
    lv_style_set_bg_color(&_styles->menuSection, lv_color_hex(_params.inactiveColor.get()));
    lv_style_set_text_color(&_styles->menuSection, lv_color_hex(_params.textColor.get()));

    styleInitReset(&_styles->menuCont);
    lv_style_set_pad_hor(&_styles->menuCont, padSmall());
    lv_style_set_pad_ver(&_styles->menuCont, padSmall());
    lv_style_set_pad_gap(&_styles->menuCont, padSmall());
    lv_style_set_border_width(&_styles->menuCont, lv_disp_dpx(_disp, 1));
    lv_style_set_border_opa(&_styles->menuCont, LV_OPA_10);
    lv_style_set_border_color(&_styles->menuCont, lv_color_hex(_params.textColor.get()));
    lv_style_set_border_side(&_styles->menuCont, LV_BORDER_SIDE_NONE);

    styleInitReset(&_styles->menuSidebarCont);
    lv_style_set_pad_all(&_styles->menuSidebarCont, 0);
    lv_style_set_pad_gap(&_styles->menuSidebarCont, 0);
    lv_style_set_border_width(&_styles->menuSidebarCont, lv_disp_dpx(_disp, 1));
    lv_style_set_border_opa(&_styles->menuSidebarCont, LV_OPA_10);
    lv_style_set_border_color(&_styles->menuSidebarCont, lv_color_hex(_params.textColor.get()));
    lv_style_set_border_side(&_styles->menuSidebarCont, LV_BORDER_SIDE_RIGHT);

    styleInitReset(&_styles->menuMainCont);
    lv_style_set_pad_all(&_styles->menuMainCont, 0);
    lv_style_set_pad_gap(&_styles->menuMainCont, 0);

    styleInitReset(&_styles->menuHeaderCont);
    lv_style_set_pad_hor(&_styles->menuHeaderCont, padSmall());
    lv_style_set_pad_ver(&_styles->menuHeaderCont, padTiny());
    lv_style_set_pad_gap(&_styles->menuHeaderCont, padSmall());

    styleInitReset(&_styles->menuHeaderBtn);
    lv_style_set_pad_hor(&_styles->menuHeaderBtn, padTiny());
    lv_style_set_pad_ver(&_styles->menuHeaderBtn, padTiny());
    lv_style_set_shadow_opa(&_styles->menuHeaderBtn, LV_OPA_TRANSP);
    lv_style_set_bg_opa(&_styles->menuHeaderBtn, LV_OPA_TRANSP);
    lv_style_set_text_color(&_styles->menuHeaderBtn, lv_color_hex(_params.textColor.get()));

    styleInitReset(&_styles->menuPage);
    lv_style_set_pad_hor(&_styles->menuPage, 0);
    lv_style_set_pad_gap(&_styles->menuPage, 0);

    styleInitReset(&_styles->menuPressed);
    lv_style_set_bg_opa(&_styles->menuPressed, LV_OPA_20);
    lv_style_set_bg_color(&_styles->menuPressed, lv_palette_main(LV_PALETTE_GREY));

    styleInitReset(&_styles->menuSeparator);
    lv_style_set_bg_opa(&_styles->menuSeparator, LV_OPA_TRANSP);
    lv_style_set_pad_ver(&_styles->menuSeparator, padTiny());
#endif

#if LV_USE_METER
    styleInitReset(&_styles->meterMarker);
    lv_style_set_line_width(&_styles->meterMarker, lv_disp_dpx(_disp, 5));
    lv_style_set_line_color(&_styles->meterMarker, lv_color_hex(_params.textColor.get()));
    lv_style_set_size(&_styles->meterMarker, lv_disp_dpx(_disp, 20));
    lv_style_set_pad_left(&_styles->meterMarker, lv_disp_dpx(_disp, 15));

    styleInitReset(&_styles->meterIndic);
    lv_style_set_radius(&_styles->meterIndic, LV_RADIUS_CIRCLE);
    lv_style_set_bg_color(&_styles->meterIndic, lv_color_hex(_params.textColor.get()));
    lv_style_set_bg_opa(&_styles->meterIndic, LV_OPA_COVER);
    lv_style_set_size(&_styles->meterIndic, lv_disp_dpx(_disp, 15));
#endif

#if LV_USE_TABLE
    styleInitReset(&_styles->tableCell);
    lv_style_set_border_width(&_styles->tableCell, lv_disp_dpx(_disp, 1));
    lv_style_set_border_color(&_styles->tableCell, lv_color_hex(_params.inactiveColor.get()));
    lv_style_set_border_side(&_styles->tableCell, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_BOTTOM);
#endif

#if LV_USE_TEXTAREA
    styleInitReset(&_styles->taCursor);
    lv_style_set_border_color(&_styles->taCursor, lv_color_hex(_params.textColor.get()));
    lv_style_set_border_width(&_styles->taCursor, lv_disp_dpx(_disp, 2));
    lv_style_set_pad_left(&_styles->taCursor, -lv_disp_dpx(_disp, 1));
    lv_style_set_border_side(&_styles->taCursor, LV_BORDER_SIDE_LEFT);
    lv_style_set_anim_time(&_styles->taCursor, 400);

    styleInitReset(&_styles->taPlaceholder);
    lv_style_set_text_color(&_styles->taPlaceholder, lv_palette_lighten(LV_PALETTE_GREY, 1));
#endif

#if LV_USE_CALENDAR
    styleInitReset(&_styles->calendarBtnmBg);
    lv_style_set_pad_all(&_styles->calendarBtnmBg, padSmall());
    lv_style_set_pad_gap(&_styles->calendarBtnmBg, padSmall() / 2);

    styleInitReset(&_styles->calendarBtnmDay);
    lv_style_set_border_width(&_styles->calendarBtnmDay, lv_disp_dpx(_disp, 1));
    lv_style_set_border_color(&_styles->calendarBtnmDay, lv_color_hex(_params.inactiveColor.get()));
    lv_style_set_bg_color(&_styles->calendarBtnmDay, lv_color_hex(_params.inactiveColor.get()));
    lv_style_set_bg_opa(&_styles->calendarBtnmDay, LV_OPA_20);

    styleInitReset(&_styles->calendarHeader);
    lv_style_set_pad_hor(&_styles->calendarHeader, padSmall());
    lv_style_set_pad_top(&_styles->calendarHeader, padSmall());
    lv_style_set_pad_bottom(&_styles->calendarHeader, padTiny());
    lv_style_set_pad_gap(&_styles->calendarHeader, padSmall());
#endif

#if LV_USE_COLORWHEEL
    styleInitReset(&_styles->colorwheelMain);
    lv_style_set_arc_width(&_styles->colorwheelMain, lv_disp_dpx(_disp, 10));
#endif

#if LV_USE_MSGBOX
    /*To add space for the button shadow*/
    styleInitReset(&_styles->msgboxBtnBg);
    lv_style_set_pad_all(&_styles->msgboxBtnBg, lv_disp_dpx(_disp, 4));

    styleInitReset(&_styles->msgboxBg);
    lv_style_set_max_width(&_styles->msgboxBg, lv_pct(100));

    styleInitReset(&_styles->msgboxBackdropBg);
    lv_style_set_bg_color(&_styles->msgboxBackdropBg, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_bg_opa(&_styles->msgboxBackdropBg, LV_OPA_50);
#endif
#if LV_USE_KEYBOARD
    styleInitReset(&_styles->keyboardBtnBg);
    lv_style_set_shadow_width(&_styles->keyboardBtnBg, 0);
    lv_style_set_radius(&_styles->keyboardBtnBg,
                        _dispSize == DispSize::dispSmall ? radiusDefault() / 2 : radiusDefault());
#endif

#if LV_USE_TABVIEW
    styleInitReset(&_styles->tabBtn);
    lv_style_set_border_color(&_styles->tabBtn, lv_color_hex(_params.mainColor.get()));
    lv_style_set_border_width(&_styles->tabBtn, _params.borderWidth.get() * 2);
    lv_style_set_border_side(&_styles->tabBtn, LV_BORDER_SIDE_BOTTOM);

    styleInitReset(&_styles->tabBgFocus);
    lv_style_set_outline_pad(&_styles->tabBgFocus, -_params.borderWidth.get());
#endif

#if LV_USE_LIST
    styleInitReset(&_styles->listBg);
    lv_style_set_pad_hor(&_styles->listBg, padDef());
    lv_style_set_pad_ver(&_styles->listBg, 0);
    lv_style_set_pad_gap(&_styles->listBg, 0);
    lv_style_set_clip_corner(&_styles->listBg, true);

    styleInitReset(&_styles->listBtn);
    lv_style_set_border_width(&_styles->listBtn, lv_disp_dpx(_disp, 1));
    lv_style_set_border_color(&_styles->listBtn, lv_color_hex(_params.inactiveColor.get()));
    lv_style_set_border_side(&_styles->listBtn, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_pad_all(&_styles->listBtn, padSmall());
    lv_style_set_pad_column(&_styles->listBtn, padSmall());

    styleInitReset(&_styles->listItemGrow);
    lv_style_set_transform_width(&_styles->listItemGrow, padDef());
#endif

#if LV_USE_LED
    styleInitReset(&_styles->led);
    lv_style_set_bg_opa(&_styles->led, LV_OPA_COVER);
    lv_style_set_bg_color(&_styles->led, lv_color_white());
    lv_style_set_bg_grad_color(&_styles->led, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_radius(&_styles->led, LV_RADIUS_CIRCLE);
    lv_style_set_shadow_width(&_styles->led, lv_disp_dpx(_disp, 15));
    lv_style_set_shadow_color(&_styles->led, lv_color_white());
    lv_style_set_shadow_spread(&_styles->led, lv_disp_dpx(_disp, 5));
#endif
}

void Theme::lvInit()
{
    if (LV_HOR_RES <= 320)
        _dispSize = DispSize::dispSmall;
    else if (LV_HOR_RES < 720)
        _dispSize = DispSize::dispMedium;
    else
        _dispSize = DispSize::dispLarge;

    UniqueLock locker(LvglPlus::lvglMutex());
    if (!_styles)
    {
        _initialized = false;
        _styles = std::make_shared<StyleContainer>();
    }
    styleInit();

    _initialized = true;
}

void Theme::replaceStyle(lv_obj_t *obj, lv_style_t *style, lv_style_selector_t selector) const
{
    UniqueLock locker(LvglPlus::lvglMutex());
    if (_applied)
    {
        lv_obj_remove_style(obj, style, selector);
        if (_removing)
        {
            lv_style_reset(style);
        }
    }
    if (!_removing)
    {
        lv_obj_add_style(obj, style, selector);
    }
}

void Theme::themeApply(lv_obj_t *obj)
{
    if (lv_obj_get_parent(obj) == nullptr)
    {
        replaceStyle(obj, &_styles->scr, 0);
        replaceStyle(obj, &_styles->scrollbar, LV_PART_SCROLLBAR);
        replaceStyle(obj, &_styles->scrollbarScrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
        return;
    }

    if (lv_obj_check_type(obj, &lv_obj_class))
    {
#if LV_USE_TABVIEW
        lv_obj_t *parent = lv_obj_get_parent(obj);
        /*Tabview content area*/
        if (lv_obj_check_type(parent, &lv_tabview_class))
        {
            return;
        }
        /*Tabview pages*/
        else if (lv_obj_check_type(lv_obj_get_parent(parent), &lv_tabview_class))
        {
            replaceStyle(obj, &_styles->padNormal, 0);
            replaceStyle(obj, &_styles->scrollbar, LV_PART_SCROLLBAR);
            replaceStyle(obj, &_styles->scrollbarScrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
            return;
        }
#endif

#if LV_USE_WIN
        /*Header*/
        if (lv_obj_get_index(obj) == 0 && lv_obj_check_type(lv_obj_get_parent(obj), &lv_win_class))
        {
            replaceStyle(obj, &_styles->bgColorGrey, 0);
            replaceStyle(obj, &_styles->padTiny, 0);
            return;
        }
        /*Content*/
        else if (lv_obj_get_index(obj) == 1 && lv_obj_check_type(lv_obj_get_parent(obj), &lv_win_class))
        {
            replaceStyle(obj, &_styles->scr, 0);
            replaceStyle(obj, &_styles->padNormal, 0);
            replaceStyle(obj, &_styles->scrollbar, LV_PART_SCROLLBAR);
            replaceStyle(obj, &_styles->scrollbarScrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
            return;
        }
#endif


#if LV_USE_CALENDAR
        if (lv_obj_check_type(lv_obj_get_parent(obj), &lv_calendar_class))
        {
            /*No style*/
            return;
        }
#endif

        replaceStyle(obj, &_styles->card, 0);
        replaceStyle(obj, &_styles->layout, 0);
        replaceStyle(obj, &_styles->scrollbar, LV_PART_SCROLLBAR);
        replaceStyle(obj, &_styles->scrollbarScrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
    }
#if LV_USE_BTN
    else if (lv_obj_check_type(obj, &lv_btn_class))
    {
        replaceStyle(obj, &_styles->btn, 0);
        replaceStyle(obj, &_styles->bgColorPrimary, 0);
        replaceStyle(obj, &_styles->transitionDelayed, 0);
        replaceStyle(obj, &_styles->pressed, LV_STATE_PRESSED);
        replaceStyle(obj, &_styles->transitionNormal, LV_STATE_PRESSED);
        replaceStyle(obj, &_styles->outlinePrimary, LV_STATE_FOCUS_KEY);
#if LV_THEME_DEFAULT_GROW
        replaceStyle(obj, &_styles->grow, LV_STATE_PRESSED);
#endif
        replaceStyle(obj, &_styles->bgColorSecondary, LV_STATE_CHECKED);
        replaceStyle(obj, &_styles->disabled, LV_STATE_DISABLED);

#if LV_USE_MENU
        if (lv_obj_check_type(lv_obj_get_parent(obj), &lv_menu_sidebar_header_cont_class) ||
            lv_obj_check_type(lv_obj_get_parent(obj), &lv_menu_main_header_cont_class))
        {
            replaceStyle(obj, &_styles->menuHeaderBtn, 0);
            replaceStyle(obj, &_styles->menuPressed, LV_STATE_PRESSED);
        }
#endif
    }
#endif

#if LV_USE_LINE
    else if (lv_obj_check_type(obj, &lv_line_class))
    {
        replaceStyle(obj, &_styles->line, 0);
    }
#endif

#if LV_USE_BTNMATRIX
    else if (lv_obj_check_type(obj, &lv_btnmatrix_class))
    {
#if LV_USE_MSGBOX
        if (lv_obj_check_type(lv_obj_get_parent(obj), &lv_msgbox_class))
        {
            replaceStyle(obj, &_styles->msgboxBtnBg, 0);
            replaceStyle(obj, &_styles->padGap, 0);
            replaceStyle(obj, &_styles->btn, LV_PART_ITEMS);
            replaceStyle(obj, &_styles->pressed, LV_PART_ITEMS | LV_STATE_PRESSED);
            replaceStyle(obj, &_styles->disabled, LV_PART_ITEMS | LV_STATE_DISABLED);
            replaceStyle(obj, &_styles->bgColorPrimary, LV_PART_ITEMS | LV_STATE_CHECKED);
            replaceStyle(obj, &_styles->bgColorPrimaryMuted, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
            replaceStyle(obj, &_styles->bgColorSecondaryMuted, LV_PART_ITEMS | LV_STATE_EDITED);
            return;
        }
#endif
#if LV_USE_TABVIEW
        if (lv_obj_check_type(lv_obj_get_parent(obj), &lv_tabview_class))
        {
            replaceStyle(obj, &_styles->bgColorWhite, 0);
            replaceStyle(obj, &_styles->outlinePrimary, LV_STATE_FOCUS_KEY);
            replaceStyle(obj, &_styles->tabBgFocus, LV_STATE_FOCUS_KEY);
            replaceStyle(obj, &_styles->pressed, LV_PART_ITEMS | LV_STATE_PRESSED);
            replaceStyle(obj, &_styles->bgColorPrimaryMuted, LV_PART_ITEMS | LV_STATE_CHECKED);
            replaceStyle(obj, &_styles->tabBtn, LV_PART_ITEMS | LV_STATE_CHECKED);
            replaceStyle(obj, &_styles->outlinePrimary, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
            replaceStyle(obj, &_styles->outlineSecondary, LV_PART_ITEMS | LV_STATE_EDITED);
            replaceStyle(obj, &_styles->tabBgFocus, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
            return;
        }
#endif

#if LV_USE_CALENDAR
        if (lv_obj_check_type(lv_obj_get_parent(obj), &lv_calendar_class))
        {
            replaceStyle(obj, &_styles->calendarBtnmBg, 0);
            replaceStyle(obj, &_styles->outlinePrimary, LV_STATE_FOCUS_KEY);
            replaceStyle(obj, &_styles->outlineSecondary, LV_STATE_EDITED);
            replaceStyle(obj, &_styles->calendarBtnmDay, LV_PART_ITEMS);
            replaceStyle(obj, &_styles->pressed, LV_PART_ITEMS | LV_STATE_PRESSED);
            replaceStyle(obj, &_styles->disabled, LV_PART_ITEMS | LV_STATE_DISABLED);
            replaceStyle(obj, &_styles->outlinePrimary, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
            replaceStyle(obj, &_styles->outlineSecondary, LV_PART_ITEMS | LV_STATE_EDITED);
            return;
        }
#endif
        replaceStyle(obj, &_styles->card, 0);
        replaceStyle(obj, &_styles->outlinePrimary, LV_STATE_FOCUS_KEY);
        replaceStyle(obj, &_styles->outlineSecondary, LV_STATE_EDITED);
        replaceStyle(obj, &_styles->btn, LV_PART_ITEMS);
        replaceStyle(obj, &_styles->disabled, LV_PART_ITEMS | LV_STATE_DISABLED);
        replaceStyle(obj, &_styles->pressed, LV_PART_ITEMS | LV_STATE_PRESSED);
        replaceStyle(obj, &_styles->bgColorPrimary, LV_PART_ITEMS | LV_STATE_CHECKED);
        replaceStyle(obj, &_styles->outlinePrimary, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
        replaceStyle(obj, &_styles->outlineSecondary, LV_PART_ITEMS | LV_STATE_EDITED);
    }
#endif

#if LV_USE_BAR
    else if (lv_obj_check_type(obj, &lv_bar_class))
    {
        replaceStyle(obj, &_styles->circle, 0);
        replaceStyle(obj, &_styles->outlinePrimary, LV_STATE_FOCUS_KEY);
        replaceStyle(obj, &_styles->outlineSecondary, LV_STATE_EDITED);
        replaceStyle(obj, &_styles->bgColorPrimary, LV_PART_INDICATOR);
        replaceStyle(obj, &_styles->circle, LV_PART_INDICATOR);
        replaceStyle(obj, &_styles->trackColor, 0);
    }
#endif

#if LV_USE_SLIDER
    else if (lv_obj_check_type(obj, &lv_slider_class))
    {
        replaceStyle(obj, &_styles->circle, 0);
        replaceStyle(obj, &_styles->outlinePrimary, LV_STATE_FOCUS_KEY);
        replaceStyle(obj, &_styles->outlineSecondary, LV_STATE_EDITED);
        replaceStyle(obj, &_styles->bgColorPrimary, LV_PART_INDICATOR);
        replaceStyle(obj, &_styles->circle, LV_PART_INDICATOR);
        replaceStyle(obj, &_styles->knob, LV_PART_KNOB);
        replaceStyle(obj, &_styles->trackColor, 0);
#if LV_THEME_DEFAULT_GROW
        replaceStyle(obj, &_styles->grow, LV_PART_KNOB | LV_STATE_PRESSED);
#endif
        replaceStyle(obj, &_styles->transitionDelayed, LV_PART_KNOB);
        replaceStyle(obj, &_styles->transitionNormal, LV_PART_KNOB | LV_STATE_PRESSED);
    }
#endif

#if LV_USE_TABLE
    else if (lv_obj_check_type(obj, &lv_table_class))
    {
        replaceStyle(obj, &_styles->card, 0);
        replaceStyle(obj, &_styles->padZero, 0);
        replaceStyle(obj, &_styles->noRadius, 0);
        replaceStyle(obj, &_styles->outlinePrimary, LV_STATE_FOCUS_KEY);
        replaceStyle(obj, &_styles->outlineSecondary, LV_STATE_EDITED);
        replaceStyle(obj, &_styles->scrollbar, LV_PART_SCROLLBAR);
        replaceStyle(obj, &_styles->scrollbarScrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
        replaceStyle(obj, &_styles->bgColorWhite, LV_PART_ITEMS);
        replaceStyle(obj, &_styles->tableCell, LV_PART_ITEMS);
        replaceStyle(obj, &_styles->padNormal, LV_PART_ITEMS);
        replaceStyle(obj, &_styles->pressed, LV_PART_ITEMS | LV_STATE_PRESSED);
        replaceStyle(obj, &_styles->bgColorPrimary, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
        replaceStyle(obj, &_styles->bgColorSecondary, LV_PART_ITEMS | LV_STATE_EDITED);
    }
#endif

#if LV_USE_CHECKBOX
    else if (lv_obj_check_type(obj, &lv_checkbox_class))
    {
        replaceStyle(obj, &_styles->padGap, 0);
        replaceStyle(obj, &_styles->outlinePrimary, LV_STATE_FOCUS_KEY);
        replaceStyle(obj, &_styles->disabled, LV_PART_INDICATOR | LV_STATE_DISABLED);
        replaceStyle(obj, &_styles->cbMarker, LV_PART_INDICATOR);
        replaceStyle(obj, &_styles->bgColorPrimary, LV_PART_INDICATOR | LV_STATE_CHECKED);
        replaceStyle(obj, &_styles->cbMarkerChecked, LV_PART_INDICATOR | LV_STATE_CHECKED);
        replaceStyle(obj, &_styles->pressed, LV_PART_INDICATOR | LV_STATE_PRESSED);
#if LV_THEME_DEFAULT_GROW
        replaceStyle(obj, &_styles->grow, LV_PART_INDICATOR | LV_STATE_PRESSED);
#endif
        replaceStyle(obj, &_styles->transitionNormal, LV_PART_INDICATOR | LV_STATE_PRESSED);
        replaceStyle(obj, &_styles->transitionDelayed, LV_PART_INDICATOR);
    }
#endif

#if LV_USE_SWITCH
    else if (lv_obj_check_type(obj, &lv_switch_class))
    {
        replaceStyle(obj, &_styles->bgColorGrey, 0);
        replaceStyle(obj, &_styles->circle, 0);
        replaceStyle(obj, &_styles->animFast, 0);
        replaceStyle(obj, &_styles->disabled, LV_STATE_DISABLED);
        replaceStyle(obj, &_styles->outlinePrimary, LV_STATE_FOCUS_KEY);
        replaceStyle(obj, &_styles->bgColorPrimary, LV_PART_INDICATOR | LV_STATE_CHECKED);
        replaceStyle(obj, &_styles->circle, LV_PART_INDICATOR);
        replaceStyle(obj, &_styles->disabled, LV_PART_INDICATOR | LV_STATE_DISABLED);
        replaceStyle(obj, &_styles->knob, LV_PART_KNOB);
        replaceStyle(obj, &_styles->bgColorWhite, LV_PART_KNOB);
        replaceStyle(obj, &_styles->switchKnob, LV_PART_KNOB);
        replaceStyle(obj, &_styles->disabled, LV_PART_KNOB | LV_STATE_DISABLED);

        replaceStyle(obj, &_styles->transitionNormal, LV_PART_INDICATOR | LV_STATE_CHECKED);
        replaceStyle(obj, &_styles->transitionNormal, LV_PART_INDICATOR);
    }
#endif

#if LV_USE_CHART
    else if (lv_obj_check_type(obj, &lv_chart_class))
    {
        replaceStyle(obj, &_styles->card, 0);
        replaceStyle(obj, &_styles->padSmall, 0);
        replaceStyle(obj, &_styles->chartBg, 0);
        replaceStyle(obj, &_styles->scrollbar, LV_PART_SCROLLBAR);
        replaceStyle(obj, &_styles->scrollbarScrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
        replaceStyle(obj, &_styles->chartSeries, LV_PART_ITEMS);
        replaceStyle(obj, &_styles->chartIndic, LV_PART_INDICATOR);
        replaceStyle(obj, &_styles->chartTicks, LV_PART_TICKS);
        replaceStyle(obj, &_styles->chartSeries, LV_PART_CURSOR);
    }
#endif

#if LV_USE_ROLLER
    else if (lv_obj_check_type(obj, &lv_roller_class))
    {
        replaceStyle(obj, &_styles->card, 0);
        replaceStyle(obj, &_styles->anim, 0);
        replaceStyle(obj, &_styles->lineSpaceLarge, 0);
        replaceStyle(obj, &_styles->textAlignCenter, 0);
        replaceStyle(obj, &_styles->outlinePrimary, LV_STATE_FOCUS_KEY);
        replaceStyle(obj, &_styles->outlineSecondary, LV_STATE_EDITED);
        replaceStyle(obj, &_styles->bgColorPrimary, LV_PART_SELECTED);
    }
#endif

#if LV_USE_DROPDOWN
    else if (lv_obj_check_type(obj, &lv_dropdown_class))
    {
        replaceStyle(obj, &_styles->card, 0);
        replaceStyle(obj, &_styles->padSmall, 0);
        replaceStyle(obj, &_styles->transitionDelayed, 0);
        replaceStyle(obj, &_styles->transitionNormal, LV_STATE_PRESSED);
        replaceStyle(obj, &_styles->pressed, LV_STATE_PRESSED);
        replaceStyle(obj, &_styles->outlinePrimary, LV_STATE_FOCUS_KEY);
        replaceStyle(obj, &_styles->outlineSecondary, LV_STATE_EDITED);
        replaceStyle(obj, &_styles->transitionNormal, LV_PART_INDICATOR);
    }
    else if (lv_obj_check_type(obj, &lv_dropdownlist_class))
    {
        replaceStyle(obj, &_styles->card, 0);
        replaceStyle(obj, &_styles->clipCorner, 0);
        replaceStyle(obj, &_styles->lineSpaceLarge, 0);
        replaceStyle(obj, &_styles->dropdownList, 0);
        replaceStyle(obj, &_styles->scrollbar, LV_PART_SCROLLBAR);
        replaceStyle(obj, &_styles->scrollbarScrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
        replaceStyle(obj, &_styles->bgColorWhite, LV_PART_SELECTED);
        replaceStyle(obj, &_styles->bgColorPrimary, LV_PART_SELECTED | LV_STATE_CHECKED);
        replaceStyle(obj, &_styles->pressed, LV_PART_SELECTED | LV_STATE_PRESSED);
    }
#endif

#if LV_USE_ARC
    else if (lv_obj_check_type(obj, &lv_arc_class))
    {
        replaceStyle(obj, &_styles->arcTrack, 0);
        replaceStyle(obj, &_styles->arcIndic, LV_PART_INDICATOR);
        replaceStyle(obj, &_styles->arcIndicPrimary, LV_PART_INDICATOR);
        replaceStyle(obj, &_styles->knob, LV_PART_KNOB);
    }
#endif


#if LV_USE_SPINNER
    else if (lv_obj_check_type(obj, &lv_spinner_class))
    {
        replaceStyle(obj, &_styles->arcTrack, 0);
        replaceStyle(obj, &_styles->arcIndic, LV_PART_INDICATOR);
        replaceStyle(obj, &_styles->arcIndicPrimary, LV_PART_INDICATOR);
    }
#endif

#if LV_USE_METER
    else if (lv_obj_check_type(obj, &lv_meter_class))
    {
        replaceStyle(obj, &_styles->card, 0);
        replaceStyle(obj, &_styles->circle, 0);
        replaceStyle(obj, &_styles->padTiny, 0);
        replaceStyle(obj, &_styles->meterIndic, LV_PART_INDICATOR);
    }
#endif

#if LV_USE_TEXTAREA
    else if (lv_obj_check_type(obj, &lv_textarea_class))
    {
        replaceStyle(obj, &_styles->card, 0);
        replaceStyle(obj, &_styles->padTiny, 0);
        replaceStyle(obj, &_styles->disabled, LV_STATE_DISABLED);
        replaceStyle(obj, &_styles->outlinePrimary, LV_STATE_FOCUS_KEY);
        replaceStyle(obj, &_styles->outlineSecondary, LV_STATE_EDITED);
        replaceStyle(obj, &_styles->scrollbar, LV_PART_SCROLLBAR);
        replaceStyle(obj, &_styles->scrollbarScrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
        replaceStyle(obj, &_styles->taCursor, LV_PART_CURSOR | LV_STATE_FOCUSED);
        replaceStyle(obj, &_styles->taPlaceholder, LV_PART_TEXTAREA_PLACEHOLDER);
    }
#endif

#if LV_USE_CALENDAR
    else if (lv_obj_check_type(obj, &lv_calendar_class))
    {
        replaceStyle(obj, &_styles->card, 0);
        replaceStyle(obj, &_styles->padZero, 0);
    }
#endif

#if LV_USE_CALENDAR_HEADER_ARROW
    else if (lv_obj_check_type(obj, &lv_calendar_header_arrow_class))
    {
        replaceStyle(obj, &_styles->calendarHeader, 0);
    }
#endif

#if LV_USE_CALENDAR_HEADER_DROPDOWN
    else if (lv_obj_check_type(obj, &lv_calendar_header_dropdown_class))
    {
        replaceStyle(obj, &_styles->calendarHeader, 0);
    }
#endif

#if LV_USE_KEYBOARD
    else if (lv_obj_check_type(obj, &lv_keyboard_class))
    {
        replaceStyle(obj, &_styles->scr, 0);
        replaceStyle(obj, _dispSize == DispSize::dispLarge ? &_styles->padSmall : &_styles->padTiny, 0);
        replaceStyle(obj, &_styles->outlinePrimary, LV_STATE_FOCUS_KEY);
        replaceStyle(obj, &_styles->outlineSecondary, LV_STATE_EDITED);
        replaceStyle(obj, &_styles->btn, LV_PART_ITEMS);
        replaceStyle(obj, &_styles->disabled, LV_PART_ITEMS | LV_STATE_DISABLED);
        replaceStyle(obj, &_styles->bgColorWhite, LV_PART_ITEMS);
        replaceStyle(obj, &_styles->keyboardBtnBg, LV_PART_ITEMS);
        replaceStyle(obj, &_styles->pressed, LV_PART_ITEMS | LV_STATE_PRESSED);
        replaceStyle(obj, &_styles->bgColorGrey, LV_PART_ITEMS | LV_STATE_CHECKED);
        replaceStyle(obj, &_styles->bgColorPrimaryMuted, LV_PART_ITEMS | LV_STATE_FOCUS_KEY);
        replaceStyle(obj, &_styles->bgColorSecondaryMuted, LV_PART_ITEMS | LV_STATE_EDITED);
    }
#endif
#if LV_USE_LIST
    else if (lv_obj_check_type(obj, &lv_list_class))
    {
        replaceStyle(obj, &_styles->card, 0);
        replaceStyle(obj, &_styles->listBg, 0);
        replaceStyle(obj, &_styles->scrollbar, LV_PART_SCROLLBAR);
        replaceStyle(obj, &_styles->scrollbarScrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
        return;
    }
    else if (lv_obj_check_type(obj, &lv_list_text_class))
    {
        replaceStyle(obj, &_styles->bgColorGrey, 0);
        replaceStyle(obj, &_styles->listItemGrow, 0);
    }
    else if (lv_obj_check_type(obj, &lv_list_btn_class))
    {
        replaceStyle(obj, &_styles->bgColorWhite, 0);
        replaceStyle(obj, &_styles->listBtn, 0);
        replaceStyle(obj, &_styles->bgColorPrimary, LV_STATE_FOCUS_KEY);
        replaceStyle(obj, &_styles->listItemGrow, LV_STATE_FOCUS_KEY);
        replaceStyle(obj, &_styles->listItemGrow, LV_STATE_PRESSED);
        replaceStyle(obj, &_styles->pressed, LV_STATE_PRESSED);
    }
#endif
#if LV_USE_MENU
    else if (lv_obj_check_type(obj, &lv_menu_class))
    {
        replaceStyle(obj, &_styles->card, 0);
        replaceStyle(obj, &_styles->menuBg, 0);
    }
    else if (lv_obj_check_type(obj, &lv_menu_sidebar_cont_class))
    {
        replaceStyle(obj, &_styles->menuSidebarCont, 0);
        replaceStyle(obj, &_styles->scrollbar, LV_PART_SCROLLBAR);
        replaceStyle(obj, &_styles->scrollbarScrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
    }
    else if (lv_obj_check_type(obj, &lv_menu_main_cont_class))
    {
        replaceStyle(obj, &_styles->menuMainCont, 0);
        replaceStyle(obj, &_styles->scrollbar, LV_PART_SCROLLBAR);
        replaceStyle(obj, &_styles->scrollbarScrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
    }
    else if (lv_obj_check_type(obj, &lv_menu_cont_class))
    {
        replaceStyle(obj, &_styles->menuCont, 0);
        replaceStyle(obj, &_styles->menuPressed, LV_STATE_PRESSED);
        replaceStyle(obj, &_styles->bgColorPrimaryMuted, LV_STATE_PRESSED | LV_STATE_CHECKED);
        replaceStyle(obj, &_styles->bgColorPrimaryMuted, LV_STATE_CHECKED);
        replaceStyle(obj, &_styles->bgColorPrimary, LV_STATE_FOCUS_KEY);
    }
    else if (lv_obj_check_type(obj, &lv_menu_sidebar_header_cont_class) ||
             lv_obj_check_type(obj, &lv_menu_main_header_cont_class))
    {
        replaceStyle(obj, &_styles->menuHeaderCont, 0);
    }
    else if (lv_obj_check_type(obj, &lv_menu_page_class))
    {
        replaceStyle(obj, &_styles->menuPage, 0);
        replaceStyle(obj, &_styles->scrollbar, LV_PART_SCROLLBAR);
        replaceStyle(obj, &_styles->scrollbarScrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
    }
    else if (lv_obj_check_type(obj, &lv_menu_section_class))
    {
        replaceStyle(obj, &_styles->menuSection, 0);
    }
    else if (lv_obj_check_type(obj, &lv_menu_separator_class))
    {
        replaceStyle(obj, &_styles->menuSeparator, 0);
    }
#endif
#if LV_USE_MSGBOX
    else if (lv_obj_check_type(obj, &lv_msgbox_class))
    {
        replaceStyle(obj, &_styles->card, 0);
        replaceStyle(obj, &_styles->msgboxBg, 0);
        return;
    }
    else if (lv_obj_check_type(obj, &lv_msgbox_backdrop_class))
    {
        replaceStyle(obj, &_styles->msgboxBackdropBg, 0);
    }
#endif
#if LV_USE_SPINBOX
    else if (lv_obj_check_type(obj, &lv_spinbox_class))
    {
        replaceStyle(obj, &_styles->card, 0);
        replaceStyle(obj, &_styles->padSmall, 0);
        replaceStyle(obj, &_styles->outlinePrimary, LV_STATE_FOCUS_KEY);
        replaceStyle(obj, &_styles->outlineSecondary, LV_STATE_EDITED);
        replaceStyle(obj, &_styles->bgColorPrimary, LV_PART_CURSOR);
    }
#endif
#if LV_USE_TILEVIEW
    else if (lv_obj_check_type(obj, &lv_tileview_class))
    {
        replaceStyle(obj, &_styles->scr, 0);
        replaceStyle(obj, &_styles->scrollbar, LV_PART_SCROLLBAR);
        replaceStyle(obj, &_styles->scrollbarScrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
    }
    else if (lv_obj_check_type(obj, &lv_tileview_tile_class))
    {
        replaceStyle(obj, &_styles->scrollbar, LV_PART_SCROLLBAR);
        replaceStyle(obj, &_styles->scrollbarScrolled, LV_PART_SCROLLBAR | LV_STATE_SCROLLED);
    }
#endif

#if LV_USE_TABVIEW
    else if (lv_obj_check_type(obj, &lv_tabview_class))
    {
        replaceStyle(obj, &_styles->scr, 0);
        replaceStyle(obj, &_styles->padZero, 0);
    }
#endif

#if LV_USE_WIN
    else if (lv_obj_check_type(obj, &lv_win_class))
    {
        replaceStyle(obj, &_styles->clipCorner, 0);
    }
#endif

#if LV_USE_COLORWHEEL
    else if (lv_obj_check_type(obj, &lv_colorwheel_class))
    {
        replaceStyle(obj, &_styles->colorwheelMain, 0);
        replaceStyle(obj, &_styles->padNormal, 0);
        replaceStyle(obj, &_styles->bgColorWhite, LV_PART_KNOB);
        replaceStyle(obj, &_styles->padNormal, LV_PART_KNOB);
    }
#endif

#if LV_USE_LED
    else if (lv_obj_check_type(obj, &lv_led_class))
    {
        replaceStyle(obj, &_styles->led, 0);
    }
#endif

    lv_obj_invalidate(obj);
}

void Theme::styleInitReset(lv_style_t *style) const
{
    UniqueLock locker(LvglPlus::lvglMutex());
    if (_initialized)
    {
        lv_style_reset(style);
    }
    else
    {
        lv_style_init(style);
    }
}

void Theme::applyThemeToChildren(lv_obj_t *parent)
{
    UniqueLock locker(LvglPlus::lvglMutex());
    for (uint32_t i = 0; i < lv_obj_get_child_cnt(parent); i++)
    {
        lv_obj_t *child = lv_obj_get_child(parent, static_cast<int32_t>(i));
        applyThemeToChildren(child);
    }
    themeApply(parent);
    _applied = true;
}

void Theme::removeThemeFromChildren(lv_obj_t *parent)
{
    UniqueLock locker(LvglPlus::lvglMutex());
    _removing = true;
    if (lv_disp_get_default())
    {
        applyThemeToChildren(parent);
    }
    _removing = false;
    _applied = false;
}

} // namespace lvglplus
} // namespace softeq