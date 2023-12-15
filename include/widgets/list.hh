#ifndef LVGL_PLUS_LIST_HH
#define LVGL_PLUS_LIST_HH

#include "properties/implementation/list_params.hh"
#include "widget.hh"
#include "widgets/button.hh"
#include "widgets/layouts/column.hh"
#include "widgets/text.hh"
#include <utility>

namespace softeq
{
namespace lvglplus
{

class List : public Widget
{
public:
    List();
    explicit List(const ListParamsBuilder &params);
    ~List() override;
    std::shared_ptr<Text> addText(std::string text);
    std::shared_ptr<Text> addText(std::string text, const TextParamsBuilder &);
    std::shared_ptr<Button> addButton(std::string text);
    std::shared_ptr<Button> addButton(std::string text, const ButtonParamsBuilder &params);

private:
    std::shared_ptr<Widget> addChild(const std::shared_ptr<Widget> &childWidget) override
    {
        return nullptr;
    }
    void render() override;
    void addSeparator();
};

} // namespace lvglplus
} // namespace softeq

#endif //LVGL_PLUS_LIST_HH
