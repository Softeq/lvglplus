#include "widgets/message_box.hh"
#include "widgets/button.hh"
#include "widgets/layouts/column.hh"
#include "widgets/layouts/container.hh"
#include "widgets/layouts/row.hh"
#include "widgets/properties/symbols.hh"

namespace softeq
{
namespace lvglplus
{

const short MessageBoxParams::defaultHeight = LV_SIZE_CONTENT;
const short MessageBoxParams::defaultWidth = 240;
const short MessageBoxParams::defaultButtonHeight = 30;
const short MessageBoxParams::defaultBackgroundOpacity = 255;
const short MessageBoxParams::defaultBorderWidth = 2;
const Padding MessageBoxParams::defaultPadding = Padding(15).row(10);
const Alignment MessageBoxParams::defaultAlignment = Alignment::center;

MessageBox::MessageBox(const std::shared_ptr<LvglDisplay> &display, const MessageBoxParamsBuilder &params)
    : Widget("MessageBox", params.getParams(), nullptr)
{
    auto messageBoxParams = params.getMessageBoxParams();
    _onCloseCallback = messageBoxParams.onCloseCallback;
    auto lvObject = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_disp_get_layer_top(display->lvDisplay()));
    LV_ASSERT_MALLOC(lvObject);
    lv_obj_class_init_obj(lvObject);
    lv_obj_clear_flag(lvObject, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(lvObject, LV_PCT(100), LV_PCT(100));
    _children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::singleChild);
    lvglObject.setObject(lvObject);

    auto window = this->addChild(std::make_shared<Column>(ColumnParamsBuilder(params.getParams())));
    if (window->backgroundOpacity() == 0)
    {
        window->setBackgroundOpacity(LV_OPA_COVER);
    }
    auto titleRow = window->addChild(std::make_shared<Row>(RowParamsBuilder().widthPctOfParent(100)));
    lv_obj_set_flex_align(titleRow->lvglObject.getObject(), LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_END,
                          LV_FLEX_ALIGN_END);
    titleRow->addChild(std::make_shared<Text>(messageBoxParams.title.text, messageBoxParams.title.textParams));

    if (messageBoxParams.closeButtonEnabled)
    {
        titleRow->addChild(std::make_shared<Button>(Symbols::CLOSE,
                                                    ButtonParamsBuilder().width(20).height(20).onClickHandler([this]() {
                                                        if (_onCloseCallback)
                                                        {
                                                            _onCloseCallback();
                                                        }
                                                        _onExitCallback();
                                                    })));
    }
    window->addChild(std::make_shared<Text>(messageBoxParams.message.text, messageBoxParams.message.textParams));

    auto row = window->addChild(std::make_shared<Row>(RowParamsBuilder().padding(Padding().column(10))));
    for (const auto &button : *messageBoxParams.button)
    {
        auto buttonWidget =
            std::static_pointer_cast<Button>(row->addChild(std::make_shared<Button>(button.text, button.buttonParams)));
        auto btnCb = button.buttonParams.getButtonParams().onClickCallback;
        buttonWidget->setOnClickHandler([btnCb, this]() {
            btnCb();
            _onExitCallback();
        });
    }
    lv_obj_center(lvObject);
}

MessageBox::MessageBox(const std::shared_ptr<LvglDisplay> &display)
    : MessageBox(display, MessageBoxParamsBuilder())
{
}

void MessageBox::render()
{
}

} // namespace lvglplus
} // namespace softeq