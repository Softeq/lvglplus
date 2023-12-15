#ifndef LVGL_PLUS_MENU_BUTTON_HH
#define LVGL_PLUS_MENU_BUTTON_HH

#include "widgets/button.hh"

namespace slvglplus = softeq::lvglplus;

class MenuButton : public slvglplus::Button
{
public:
    explicit MenuButton(const slvglplus::ButtonParamsBuilder &params)
        : Button(params)
    {
        _children.reset();
        _children = slvglplus::WidgetContainer::MakeWidgetContainer(slvglplus::WidgetContainerType::multiChild);
        auto lineParams = slvglplus::ContainerParamsBuilder()
                              .width(25)
                              .height(2)
                              .backgroundColor(0xffffff)
                              .alignment(slvglplus::Alignment::center)
                              .radius(2);
        this->addChild(std::make_shared<slvglplus::Container>(lineParams));
        this->addChild(std::make_shared<slvglplus::Container>(lineParams.offset(0, -7)));
        this->addChild(std::make_shared<slvglplus::Container>(lineParams.offset(0, 7)));
        setWidth(70);
        setHeight(32);
        setBackgroundColor(0x14191E);
    }
    MenuButton()
        : MenuButton(slvglplus::ButtonParamsBuilder())
    {
    }
};

#endif //LVGL_PLUS_MENU_BUTTON_HH
