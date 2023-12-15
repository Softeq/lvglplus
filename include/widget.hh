#ifndef LVGL_WIDGET_HH
#define LVGL_WIDGET_HH

#include "lvgl.h"
#include "lvgl_object.hh"
#include "renderable.hh"
#include "widget_container.hh"
#include "widgets/properties/implementation/widget_params.hh"
#include <iostream>
#include <list>
#include <mutex>

namespace softeq
{
namespace lvglplus
{
using UniqueLock = std::unique_lock<std::recursive_mutex>;
using Offset = Point;

class Widget : public Renderable, public std::enable_shared_from_this<Widget>
{
public:
    std::shared_ptr<Widget> root()
    {
        if (_parent)
            return _parent->root();
        else
            return shared_from_this();
    }
    /// *** Constructors/destructor ***
    Widget();
    explicit Widget(std::string name);
    Widget(std::string name, const WidgetParams &params);
    Widget(std::string name, const WidgetParams &params, const std::function<lv_obj_t *(lv_obj_t *)> &createFunc);
    Widget(std::string name, const WidgetParams &params, lv_obj_t *lvObject);
    Widget(std::string name, lv_obj_t *lvObject);
    ~Widget();
    bool isRendered() const
    {
        return _rendered;
    }
    /// Operations with children
    virtual std::shared_ptr<Widget> addChild(const std::shared_ptr<Widget> &newWidget);
    virtual std::shared_ptr<Widget> replaceChild(const std::shared_ptr<Widget> &newWidget,
                                                 const std::shared_ptr<Widget> &oldWidget = nullptr);
    virtual std::shared_ptr<Widget> removeChild(const std::shared_ptr<Widget> &childWidget = nullptr);

    /// Diagnostics
    /**
     * @brief Generates an instance name of the widget to simplify debugging.
     *
     * @return std::string An instance name of the widget.
     */
    std::string toString();

    /**
     * @brief Generates a visual representation of the widget tree from the root widget and mark current widget to
     * highlight to simplify searching (if needed).
     *
     * @param markWidget A flag to add string-arrow to highlight current widget.
     * @return std::string A visual representation of the whole tree.
     */
    std::string wholeTreeToString(bool markWidget = true);

    /**
     * @brief Generates a visual representation of the widget tree and mark another widget to highlight to simplify
     * searching (if needed).
     *
     * @param widgetToMark A widget that needs to be marked to highlight.
     * @return std::string A visual representation of thetree.
     */
    std::string subtreeToString(const std::shared_ptr<Widget> &widgetToMark = nullptr);

    void setDiagnosticName(const std::string &name)
    {
        _diagnosticName = name;
    }

    /// Settings
    long backgroundColor() const
    {
        return static_cast<long>(lv_obj_get_style_bg_color(lvglObject.getObject(), 0).full & 0xffffff);
    }
    void setBackgroundColor(const long &color) const
    {
        auto lvObject = lvglObject.getObject();
        if (lv_obj_get_style_bg_opa(lvObject, 0) == 0)
        {
            lv_obj_set_style_bg_opa(lvObject, 255, 0);
        }
        lv_obj_set_style_bg_color(lvObject, lv_color_hex(color), 0);
    }

    long borderColor() const
    {
        return static_cast<long>(lv_obj_get_style_border_color(lvglObject.getObject(), 0).full & 0xffffff);
    }
    void setBorderColor(const long &color) const
    {
        auto lvObject = lvglObject.getObject();
        if (lv_obj_get_style_border_opa(lvObject, 0) == 0)
        {
            lv_obj_set_style_border_opa(lvObject, 255, 0);
        }
        lv_obj_set_style_border_color(lvObject, lv_color_hex(color), 0);
    }

    unsigned short backgroundOpacity() const
    {
        return static_cast<unsigned short>(lv_obj_get_style_bg_opa(lvglObject.getObject(), 0));
    }
    void setBackgroundOpacity(const unsigned short &value) const
    {
        lv_obj_set_style_bg_opa(lvglObject.getObject(), value, 0);
    }

    unsigned short opacity() const
    {
        return static_cast<unsigned short>(lv_obj_get_style_opa(lvglObject.getObject(), 0));
    }
    void setOpacity(const unsigned short &value) const
    {
        lv_obj_set_style_opa(lvglObject.getObject(), value, 0);
    }

    Offset offset() const
    {
        Offset offset;
        offset.x = lv_obj_get_x_aligned(lvglObject.getObject());
        offset.y = lv_obj_get_y_aligned(lvglObject.getObject());
        return offset;
    }
    void setOffset(const Offset &value) const
    {
        lv_obj_set_x(lvglObject.getObject(), value.x);
        lv_obj_set_y(lvglObject.getObject(), value.y);
    }

    Padding padding() const
    {
        Padding pad;
        auto lvObject = lvglObject.getObject();
        pad.top(lv_obj_get_style_pad_top(lvObject, 0));
        pad.bottom(lv_obj_get_style_pad_bottom(lvObject, 0));
        pad.left(lv_obj_get_style_pad_left(lvObject, 0));
        pad.right(lv_obj_get_style_pad_right(lvObject, 0));
        pad.column(lv_obj_get_style_pad_column(lvObject, 0));
        pad.row(lv_obj_get_style_pad_row(lvObject, 0));
        return pad;
    }
    void setPadding(const Padding &padding) const
    {
        auto lvObject = lvglObject.getObject();
        lv_obj_set_style_pad_top(lvObject, padding.getTop(), 0);
        lv_obj_set_style_pad_bottom(lvObject, padding.getBottom(), 0);
        lv_obj_set_style_pad_left(lvObject, padding.getLeft(), 0);
        lv_obj_set_style_pad_right(lvObject, padding.getRight(), 0);
        lv_obj_set_style_pad_row(lvObject, padding.getRow(), 0);
        lv_obj_set_style_pad_column(lvObject, padding.getColumn(), 0);
    }

    Alignment alignment() const
    {
        return static_cast<Alignment>(lv_obj_get_style_align(lvglObject.getObject(), 0));
    }
    void setAlignment(const Alignment &value) const
    {
        lv_obj_set_align(lvglObject.getObject(), static_cast<uint8_t>(value));
    }

    short borderWidth() const
    {
        return static_cast<short>(lv_obj_get_style_border_width(lvglObject.getObject(), 0));
    }
    void setBorderWidth(const short &value) const
    {
        lv_obj_set_style_border_width(lvglObject.getObject(), value, 0);
    }

    short width() const
    {
        return static_cast<short>(lv_obj_get_style_width(lvglObject.getObject(), 0));
    }
    void setWidth(const short &value) const
    {
        lv_obj_set_width(lvglObject.getObject(), value);
    }

    short height() const
    {
        return static_cast<short>(lv_obj_get_style_height(lvglObject.getObject(), 0));
    }
    void setHeight(const short &value) const
    {
        lv_obj_set_height(lvglObject.getObject(), value);
    }

    short radius() const
    {
        return static_cast<short>(lv_obj_get_style_radius(lvglObject.getObject(), 0));
    }
    void setRadius(short value) const
    {
        lv_obj_set_style_radius(lvglObject.getObject(), value, 0);
    }

    const int &zIndex() const
    {
        return _zIndex;
    }
    void setZIndex(int value)
    {
        _zIndex = value;
    }

    Widget *parent()
    {
        return _parent;
    }
    void setParent(Widget *widget)
    {
        _parent = widget;
        if (_parent)
        {
            lvglObject.setParent(_parent->lvglObject.getObject());
        }
    }

    /// LVGL usage
    LvglObject lvglObject;

protected:
    /// *** Render ***
    bool _rendered = false;
    void renderWithChildren() override;
    void renderChildren();

    /// Parameters
    void applyPadding(const Padding &padding);
    std::string _widgetName;
    std::string _diagnosticName;
    int _zIndex = 0;

    /// Widget tree
    Widget *_parent = nullptr;
    lv_obj_t *getLvObjectFromParent();
    std::unique_ptr<WidgetContainer> _children = nullptr;

    /**
     * @brief Recurcively generates a visual representation of the widget tree and mark another widget to highlight to
     * simplify searching (if needed).
     *
     * @param widgetToMark A widget that needs to be marked to highlight.
     * @param indentCnt An intend counter for each step of recurcive call
     * @return std::string A visual representation of thetree.
     */
    std::string subtreeToStringRecurcive(Widget *widgetToMark, unsigned int indentCnt = 1);

    void render() override;
    void prepareWidget(const WidgetParams &params, lv_obj_t *lvObject);
};

} // namespace lvglplus
} // namespace softeq

#endif // LVGL_WIDGET_HH
