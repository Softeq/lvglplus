#include "test_fixture.hh"
#include "widget_containers/no_child_widget.hh"
#include "widgets/layouts/column.hh"
#include "widgets/layouts/container.hh"
#include "widgets/layouts/row.hh"
#include "widgets/text.hh"
#include <gtest/gtest.h>

namespace softeq
{
namespace lvglplus
{

TEST(WidgetTest, WidgetContainersChildrenManage) // checks add, replace and remove methods
{
    TestParams::getInstance();
    std::unique_ptr<WidgetContainer> multiContainer = std::unique_ptr<WidgetContainer>(new MultiChildWidget());
    multiContainer->add(std::make_shared<Container>());
    ASSERT_EQ(multiContainer->getChildrenQuantity(), 1);
    auto widgetToReplace = multiContainer->add(std::make_shared<Container>());
    ASSERT_EQ(multiContainer->getChildrenQuantity(), 2);
    multiContainer->replace(std::make_shared<Container>(), widgetToReplace);
    ASSERT_EQ(multiContainer->getChildrenQuantity(), 2);
    multiContainer->replace(std::make_shared<Container>(), nullptr);
    ASSERT_EQ(multiContainer->getChildrenQuantity(), 1);
    multiContainer->remove(nullptr);
    ASSERT_EQ(multiContainer->getChildrenQuantity(), 0);

    std::unique_ptr<WidgetContainer> singleContainer = std::unique_ptr<WidgetContainer>(new SingleChildWidget());
    singleContainer->add(std::make_shared<Container>());
    ASSERT_EQ(singleContainer->getChildrenQuantity(), 1);
    singleContainer->add(std::make_shared<Container>());
    ASSERT_EQ(singleContainer->getChildrenQuantity(), 1);
    singleContainer->replace(std::make_shared<Container>(), nullptr);
    ASSERT_EQ(singleContainer->getChildrenQuantity(), 1);
    widgetToReplace = singleContainer->add(std::make_shared<Container>());
    singleContainer->replace(std::make_shared<Container>(), widgetToReplace);
    ASSERT_EQ(singleContainer->getChildrenQuantity(), 1);
    singleContainer->remove(nullptr);
    ASSERT_EQ(singleContainer->getChildrenQuantity(), 0);

    std::unique_ptr<WidgetContainer> noContainer = std::unique_ptr<WidgetContainer>(new NoChildWidget());
    noContainer->add(std::make_shared<Container>());
    ASSERT_EQ(noContainer->getChildrenQuantity(), 0);
    noContainer->replace(std::make_shared<Container>(), nullptr);
    ASSERT_EQ(noContainer->getChildrenQuantity(), 0);
    noContainer->remove(nullptr);
    ASSERT_EQ(noContainer->getChildrenQuantity(), 0);
}

TEST(WidgetTest, WidgetHasParent) // Checks parent before and after adding.
{
    auto parentWidget = std::make_shared<Column>(); // make new column with no parent
    ASSERT_EQ(parentWidget->parent(), nullptr);
    auto childWidget1 =
        parentWidget->addChild(std::make_shared<Container>()); // make anoter container and add it to the column
    ASSERT_EQ(childWidget1->parent(), parentWidget.get());
    ASSERT_EQ(parentWidget->parent(), nullptr);
    auto childWidget2 = std::make_shared<Container>(); // make anoter container
    ASSERT_EQ(childWidget2->parent(), nullptr);
    parentWidget->addChild(childWidget2); // add the container to the column
    ASSERT_EQ(childWidget2->parent(), parentWidget.get());
}

TEST(WidgetTest, WidgetChildrenPointers) // Adds children and checks quantity, pointers match and order
{
    class MyWidget : public Column
    {
    public:
        MyWidget() = default;
        void addChildren(int count)
        {
            removeChild();
            std::shared_ptr<Widget> widgets[count];
            for (int i = 0; i < count; ++i)
            {
                switch (i % 3)
                {
                case 0:
                    widgets[i] = addChild(std::make_shared<Text>());
                    break;
                case 1:
                    widgets[i] = addChild(std::make_shared<Container>());
                    break;
                case 2:
                    widgets[i] = addChild(std::make_shared<Row>());
                    break;
                }
            }
            // checks children quantity in MyWidget
            ASSERT_EQ(_children->getChildrenQuantity(), count);
            int i = 0;
            // checks iterator (Text - no child, Container - single child, Row - multi child)
            for (auto iterChild = _children->begin(); *iterChild != *(_children->end()); ++(*iterChild))
            {
                ASSERT_TRUE(&(**iterChild) == widgets[i++].get());
            }
        }
    };
    MyWidget m;
    m.addChildren(0);
    m.addChildren(1);
    m.addChildren(2);
    m.addChildren(10);
}

TEST(WidgetTest, WidgetToString) // Checks widget toString() method (raw and extended with a diagnostic name)
{
    std::string diagnosticName = "text to test";
    auto myContainer = std::make_shared<Container>();
    ASSERT_TRUE(myContainer->toString().find("Container"));
    myContainer->setDiagnosticName(diagnosticName);
    ASSERT_TRUE(myContainer->toString().find("Container"));
    ASSERT_TRUE(myContainer->toString().find(diagnosticName));
}

TEST(WidgetTest, WidgetTreeToString) // Checks wholeTreeToString() contain all widgets
{
    auto root = std::make_shared<Column>();
    root->setDiagnosticName("root");
    auto widget1 = root->addChild(std::make_shared<Container>());
    widget1->setDiagnosticName("widget1");
    auto widget2 = root->addChild(std::make_shared<Row>());
    widget2->setDiagnosticName("widget2");
    auto widget3 = root->addChild(std::make_shared<Text>());
    widget3->setDiagnosticName("widget3");
    ASSERT_TRUE(root->wholeTreeToString().find(root->toString()));
    ASSERT_TRUE(root->wholeTreeToString().find(widget1->toString()));
    ASSERT_TRUE(root->wholeTreeToString().find(widget2->toString()));
    ASSERT_TRUE(root->wholeTreeToString().find(widget3->toString()));
}

} // namespace lvglplus
} // namespace softeq
