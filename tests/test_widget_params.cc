#include "test_fixture.hh"
#include "widgets/button.hh"
#include "widgets/chart.hh"
#include "widgets/checkbox.hh"
#include "widgets/circular_slider.hh"
#include "widgets/dropdown_button.hh"
#include "widgets/image.hh"
#include "widgets/layouts/column.hh"
#include "widgets/layouts/container.hh"
#include "widgets/layouts/row.hh"
#include "widgets/led.hh"
#include "widgets/list.hh"
#include "widgets/list_wheel_scroll_view.hh"
#include "widgets/meter.hh"
#include "widgets/slider.hh"
#include "widgets/spinbox.hh"
#include "widgets/spinner.hh"
#include "widgets/switch.hh"
#include "widgets/table.hh"
#include "widgets/tabview.hh"
#include "widgets/text.hh"
#include "widgets/text_field.hh"
#include <gtest/gtest.h>

namespace softeq
{
namespace lvglplus
{

static void checkWidgetBasicParams(const std::shared_ptr<Widget> &widget, const WidgetTestParams &params)
{
    // check lvgl plus parameters
    ASSERT_EQ(params.width, widget->width());
    ASSERT_EQ(params.height, widget->height());
    ASSERT_EQ(params.borderWidth, widget->borderWidth());
    ASSERT_EQ(params.borderColor, widget->borderColor());
    ASSERT_EQ(params.alignment, widget->alignment());
    ASSERT_EQ(params.backgroundColor, widget->backgroundColor());
    ASSERT_EQ(params.backgroundOpacity, widget->backgroundOpacity());
    ASSERT_EQ(params.opacity, widget->opacity());
    ASSERT_EQ(params.offset.x, widget->offset().x);
    ASSERT_EQ(params.offset.y, widget->offset().y);
    ASSERT_EQ(params.padding.getTop(), widget->padding().getTop());
    ASSERT_EQ(params.padding.getBottom(), widget->padding().getBottom());
    ASSERT_EQ(params.padding.getLeft(), widget->padding().getLeft());
    ASSERT_EQ(params.padding.getRight(), widget->padding().getRight());
    ASSERT_EQ(params.padding.getColumn(), widget->padding().getColumn());
    ASSERT_EQ(params.padding.getRow(), widget->padding().getRow());
}

template <typename ParamsBuilder>
static void basicParamsBuilder(WidgetParamsBuilder<ParamsBuilder> *widgetParamsBuilder,
                               const WidgetTestParams &customParams)
{
    widgetParamsBuilder->width(customParams.width);
    widgetParamsBuilder->height(customParams.height);
    widgetParamsBuilder->borderWidth(customParams.borderWidth);
    widgetParamsBuilder->borderColor(customParams.borderColor);
    widgetParamsBuilder->alignment(customParams.alignment);
    widgetParamsBuilder->backgroundOpacity(customParams.backgroundOpacity);
    widgetParamsBuilder->backgroundColor(customParams.backgroundColor);
    widgetParamsBuilder->opacity(customParams.opacity);
    widgetParamsBuilder->offset(customParams.offset);
    widgetParamsBuilder->padding(customParams.padding);
}

template <typename ParamsBuilder>
static void textBasicParamsBuilder(TextBasicParamsBuilder<ParamsBuilder> *widgetParamsBuilder,
                                   const TextTestParams &customParams)
{
    widgetParamsBuilder->fontColor(customParams.fontColor);
    widgetParamsBuilder->fontSize(customParams.fontSize);
    widgetParamsBuilder->fontAlign(customParams.fontAlign);
}

template <typename ParamsBuilder>
static void indicatorParamsBuilder(IndicatorParamsBuilder<ParamsBuilder> *widgetParamsBuilder,
                                   const IndicatorTestParams &customParams)
{
    widgetParamsBuilder->indicatorColor(customParams.indicatorColor);
    widgetParamsBuilder->indicatorBorderWidth(customParams.indicatorBorderWidth);
    widgetParamsBuilder->indicatorBorderColor(customParams.indicatorBorderColor);
}

template <typename ParamsBuilder>
static void knobParamsBuilder(KnobParamsBuilder<ParamsBuilder> *widgetParamsBuilder, const KnobTestParams &customParams)
{
    widgetParamsBuilder->knobSize(customParams.knobSize);
    widgetParamsBuilder->knobColor(customParams.knobColor);
    widgetParamsBuilder->knobBorderColor(customParams.knobBorderColor);
    widgetParamsBuilder->knobBorderWidth(customParams.knobBorderWidth);
}

template <typename ParamsBuilder>
static void checkWidgetWithParamsBuilder(std::function<std::shared_ptr<Widget>(ParamsBuilder *)> builderFunc)
{
    auto *paramsBuilder = new ParamsBuilder();
    static WidgetTestParams paramsToCheck;
    static TextTestParams textParamsToCheck;
    static IndicatorTestParams indicatorParamsToCheck;
    static KnobTestParams knobParamsToCheck;
    basicParamsBuilder(paramsBuilder, paramsToCheck);
    if (auto textBasicParams = dynamic_cast<TextBasicParamsBuilder<ParamsBuilder> *>(paramsBuilder))
    {
        textBasicParamsBuilder(textBasicParams, textParamsToCheck);
    }
    if (auto indicatorParams = dynamic_cast<IndicatorParamsBuilder<ParamsBuilder> *>(paramsBuilder))
    {
        indicatorParamsBuilder(indicatorParams, indicatorParamsToCheck);
    }
    if (auto konbParams = dynamic_cast<KnobParamsBuilder<ParamsBuilder> *>(paramsBuilder))
    {
        knobParamsBuilder(konbParams, knobParamsToCheck);
    }
    std::shared_ptr<Widget> myWidget = builderFunc(paramsBuilder);
    static_cast<Renderable *>(myWidget.get())->renderWithChildren();
    checkWidgetBasicParams(myWidget, paramsToCheck);
}

TEST(WidgetParamsTest, ContainerParams) // Checks Container parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new Container
    checkWidgetWithParamsBuilder<ContainerParamsBuilder>(
        [](ContainerParamsBuilder *params) -> std::shared_ptr<Widget> { return std::make_shared<Container>(*params); });
}

TEST(WidgetParamsTest, RowParams) // Checks Row parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new Row
    checkWidgetWithParamsBuilder<RowParamsBuilder>(
        [](RowParamsBuilder *params) -> std::shared_ptr<Widget> { return std::make_shared<Row>(*params); });
}

TEST(WidgetParamsTest, ColumnParams) // Checks Column parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new Column
    checkWidgetWithParamsBuilder<ColumnParamsBuilder>(
        [](ColumnParamsBuilder *params) -> std::shared_ptr<Widget> { return std::make_shared<Column>(*params); });
}

TEST(WidgetParamsTest, TextParams) // Checks Text parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new Text
    checkWidgetWithParamsBuilder<TextParamsBuilder>(
        [](TextParamsBuilder *params) -> std::shared_ptr<Widget> { return std::make_shared<Text>("text", *params); });
}

TEST(WidgetParamsTest, TextFieldParams) // Checks TextField parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new TextField
    checkWidgetWithParamsBuilder<TextFieldParamsBuilder>([](TextFieldParamsBuilder *params) -> std::shared_ptr<Widget> {
        return std::make_shared<TextField>("text", *params);
    });
}

TEST(WidgetParamsTest, ButtonParams) // Checks Button parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new Button
    checkWidgetWithParamsBuilder<ButtonParamsBuilder>(
        [](ButtonParamsBuilder *params) -> std::shared_ptr<Widget> { return std::make_shared<Button>(*params); });
}

TEST(WidgetParamsTest, SliderParams) // Checks Slider parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new Slider
    checkWidgetWithParamsBuilder<SliderParamsBuilder>(
        [](SliderParamsBuilder *params) -> std::shared_ptr<Widget> { return std::make_shared<Slider>(*params); });
}

TEST(WidgetParamsTest, CircularSliderParams) // Checks CircularSlider parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new CircularSlider
    checkWidgetWithParamsBuilder<CircularSliderParamsBuilder>(
        [](CircularSliderParamsBuilder *params) -> std::shared_ptr<Widget> {
            return std::make_shared<CircularSlider>(*params);
        });
}

TEST(WidgetParamsTest, CheckboxParams) // Checks Checkbox parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new Checkbox
    checkWidgetWithParamsBuilder<CheckboxParamsBuilder>(
        [](CheckboxParamsBuilder *params) -> std::shared_ptr<Widget> { return std::make_shared<Checkbox>(*params); });
}

TEST(WidgetParamsTest, ListWheelScrollViewParams) // Checks ListWheelScrollView parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new ListWheelScrollView
    checkWidgetWithParamsBuilder<ListWheelScrollViewParamsBuilder>(
        [](ListWheelScrollViewParamsBuilder *params) -> std::shared_ptr<Widget> {
            return std::make_shared<ListWheelScrollView>(*params);
        });
}

TEST(WidgetParamsTest, DropDownButtonParams) // Checks DropDownButton parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new DropDownButton
    checkWidgetWithParamsBuilder<DropDownButtonParamsBuilder>(
        [](DropDownButtonParamsBuilder *params) -> std::shared_ptr<Widget> {
            return std::make_shared<DropDownButton>(*params);
        });
}

TEST(WidgetParamsTest, SwitchParams) // Checks Switch parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new Switch
    checkWidgetWithParamsBuilder<SwitchParamsBuilder>(
        [](SwitchParamsBuilder *params) -> std::shared_ptr<Widget> { return std::make_shared<Switch>(*params); });
}

TEST(WidgetParamsTest, LedParams) // Checks Led parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new Led
    checkWidgetWithParamsBuilder<LedParamsBuilder>(
        [](LedParamsBuilder *params) -> std::shared_ptr<Widget> { return std::make_shared<Led>(*params); });
}

TEST(WidgetParamsTest, ChartParams) // Checks Chart parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new Chart
    checkWidgetWithParamsBuilder<ChartParamsBuilder>(
        [](ChartParamsBuilder *params) -> std::shared_ptr<Widget> { return std::make_shared<Chart>(*params); });
}

TEST(WidgetParamsTest, TableParams) // Checks Table parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new Table
    checkWidgetWithParamsBuilder<TableParamsBuilder>(
        [](TableParamsBuilder *params) -> std::shared_ptr<Widget> { return std::make_shared<Table>(*params); });
}

TEST(WidgetParamsTest, TabviewParams) // Checks Tabview parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new Tabview
    checkWidgetWithParamsBuilder<TabviewParamsBuilder>(
        [](TabviewParamsBuilder *params) -> std::shared_ptr<Widget> { return std::make_shared<Tabview>(*params); });
}

TEST(WidgetParamsTest, SpinnerParams) // Checks Spinner parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new Spinner
    checkWidgetWithParamsBuilder<SpinnerParamsBuilder>(
        [](SpinnerParamsBuilder *params) -> std::shared_ptr<Widget> { return std::make_shared<Spinner>(*params); });
}

TEST(WidgetParamsTest, MeterParams) // Checks Meter parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new Meter
    checkWidgetWithParamsBuilder<MeterParamsBuilder>(
        [](MeterParamsBuilder *params) -> std::shared_ptr<Widget> { return std::make_shared<Meter>(*params); });
}

TEST(WidgetParamsTest, ListParams) // Checks List parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new List
    checkWidgetWithParamsBuilder<ListParamsBuilder>(
        [](ListParamsBuilder *params) -> std::shared_ptr<Widget> { return std::make_shared<List>(*params); });
}

TEST(WidgetParamsTest, SpinboxParams) // Checks Spinbox parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new Spinbox
    checkWidgetWithParamsBuilder<SpinboxParamsBuilder>(
        [](SpinboxParamsBuilder *params) -> std::shared_ptr<Widget> { return std::make_shared<Spinbox>(*params); });
}

TEST(WidgetParamsTest, ImageParams) // Checks Image parameters match
{
    TestParams::getInstance();

    // Check builded basic parameters in a new Image
    checkWidgetWithParamsBuilder<ImageParamsBuilder>(
        [](ImageParamsBuilder *params) -> std::shared_ptr<Widget> { return std::make_shared<Image>(*params); });
}

} // namespace lvglplus
} // namespace softeq
