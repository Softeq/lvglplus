# LVGL Plus <a name="lvgl-plus"></a>
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

This wrapper combines Flutter-like approach to build UI, HTML-like approach to set widget properties
and general C++ principles.

Some features (like beautiful default param initialization) will be available only with C++14, but you can
use C++11 without any problems.

## Motivation <a name="motivation"></a>

UI development in pure C using LVGL is not the most pleasant experience. On the other hand,
using Flutter isn't possible for small and cheap embedded devices. So, main idea is to bring
experience of Flutter development with a lightweight C++ wrapper on top of LVGL.

## Index <a name="index"></a>

- [Index](#index)
- [How to use](#how-to-use)
    - [Install](#install)
        - [Prerequisites](#prerequisites)
        - [Build](#build)
        - [Using CLion as IDE](#clion-setup)
    - [Getting started](#getting-started)
        - [Drivers](#drivers)
          - [Display](#display)
          - [Input devices](#input-devices)
        - [Hello world example](#hello-world-example)
        - [A declaration is not a construct](#building-is-not-drawing)
        - [Apply parameters](#apply-parameters)
            - [Align](#align-param)
            - [Padding](#padding-param)
            - [Offset](#offset-param)
        - [User-defined widget creation](#user-defined-widget-creation)
        - [Interaction with widgets](#interaction-with-widgets)
            - [Widget controller](#control-by-controller)
            - [Stream](#control-by-stream)
- [Concept description](#concept-description)
    - [Inheritance tree](#inheritance-tree)
        - [Widget](#widget-class)
        - [Children container](#children-container)
        - [Debug widget tree](#debug-widget-tree)
    - [Build interface](#build-interface)
    - [Main loop (runApp)](#main-loop-runapp)
    - [Basic widget creation](#basic-widget-creation)
    - [Widget catalog](#widget-catalog)
    - [Stream widget builder](#stream-widget-builder)
    - [BLoC pattern](#bloc-pattern)
        - [BLoC widget builder](#bloc-widget-builder)
    - [Golden tests](/examples/heater/golden_tests/README.md)

# How to use <a name="how-to-use"></a>

## Install <a name="install"></a>

### Build (without docker*)<a name="build"></a>

* Clone repository `lvglplus`.
* Open the repository and use this command to get all submodules:
  `git submodule update --init --recursive --remote`
* To build all use these commands: `cmake -B build -S . && cd build && make all`


* Some examples to try with SDL:

`./build/examples/heater/heater_example`
`./build/examples/thermostat/thermostat_example`
`./build/examples/multiple_display/multiple_display_example`

***It is temporary description and algorithm, it should be updated in the future**

### Using CLion as IDE <a name="clion-setup"></a>

If you decided to use [CLIon](https://www.jetbrains.com/clion/), you have to do this steps:

* Download studio from a store (for example `Ubuntu software`)
* If you open project first time you will be on a CMake project settings screen. If you don't see it,
  go to `File` -> `Settings` -> `Build, Execution, Deployement` -> `CMake`
* Set `Generator` -> `Unix Makefiles`
* Set `Build directory` -> `build`

If you get an `sdl2 not found` error during the build process ,
visit https://wiki.libsdl.org/SDL2/Installation to install SDL2.<br>
For Linux/Unix: `sudo apt-get install libsdl2-dev`.

If you edit CMake properties after build, delete `build` directory to avoid errors.

Other useful settings:

* Open current active file in a project tree: `double shift` -> write `always` -> turn `ON` on
  `Always Select Opened File`
* Real full-screen mode: `File` -> `Settings` -> in the search write `full screen` -> assign a button
  (for example, F11) for `Toggle Full Screen mode` setting.

How to get free licence (only for education purpose):
Register on the [Stepik](https://stepik.org/) and finish a few programming tasks in a [C++](https://stepik.org/course/7)
(highly recommend to pass it), [JS](https://stepik.org/course/2223/), [Python](https://stepik.org/course/67/) or any
other programming course. In some time you'll get a 3-month licence. Also, you can solve a couple of tasks after 3 month
and get new licence.

## Getting started <a name="getting-started"></a>

### Drivers <a name="drivers"></a>

#### Display <a name="display"></a>

To test displays and drivers on a computer you can use SDL library. There is a prepared set to work with SDL that can
be included with followed code:

```c++
#include "helpers/sdl/sdl_drv.hh"
...
addSdlPeripherals(300, 400);
```

It includes SDL display and drivers for keyboard and mouse. There is can be more than one display and each one can have
own driver. To add custom display driver you can use static method `addDisplay` from LvglPlus library. LvglPlus will
create all needed LVGL objects, but you need to create a callback function to draw the buffer.

Here is an example from the official [documentation](https://docs.lvgl.io/master/porting/disp.html):

```c++
void my_flush_cb(lv_disp_t * disp, const lv_area_t * area, lv_color_t * buf)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one
     *`put_px` is just an example, it needs to be implemented by you.*/
    int32_t x, y;
    for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            put_px(x, y, *color_p);
            color_p++;
        }
    }

    /* IMPORTANT!!!
     * Inform LVGL that you are ready with the flushing and buf is not used anymore*/
    lv_disp_flush_ready(disp);
}
```

Here is an example how to add display:

```c++
LvglPlus::addDisplay(width, height, my_flush_cb);
```

Widgets can be set as a screen for the display (by default the widget will be added to the last created diaplay):

```c++
std::shared_ptr<Widget> myScreen = std::make_shared<Container>();
LvglPlus::setScreen(myScreen);
```

If you want to use several displays, you need to add another callback and call `addDisplay` method.

To test it with SDL you can call helper function `addSdlPeripherals` and specify a size of the screen and which drivers
to connect. By default, LVGL plus will connect mouse, mouse wheel and keyboard. But you can disable some drivers by 
writing false value to the corresponding argements.

```c++
addSdlPeripherals(300, 400, true, false, false); // enable mouse, disable mouse wheel and keyboard
```

To specify the display to set a sreen for, use `setScreen` method with shared pointer to `LvglDisplay` as the second
argument:

```c++
std::shared_ptr<LvglDisplay> display = LvglPlus::addDisplay(width, height, my_flush_cb);
LvglPlus::setScreen(myScreen, display);
```

You can find a display by id, that is incremented for each created display (e.g. the first added display has id 0,
second - 1, etc.).

```c++
addSdlPeripherals(300, 400);
LvglPlus::setScreen(myScreen, LvglPlus::findDisplay(1));
```

#### Input devices <a name="input-devices"></a>

To add custom input driver you can use static methods `addMouse`, `addEncoder`, `addKeyboard`, `addButton`, `addDriver`
from LvglPlus library. LvglPlus will create all needed LVGL objects, but you need to create a callback function to send
input data to LVGL.

To see examples how to create callbacks, please visit
official [documentation](https://docs.lvgl.io/master/porting/indev.html).

### Hello world example <a name="hello-world-example"></a>

By using this guide, you can view some examples and understand how to work with this library.

Let's see this minimal example:

```c++
#include "lvgl_plus.hh"
#include "widgets/text.hh"
#include "helpers/sdl/sdl_drv.hh"

using namespace softeq::lvglplus;

int main(int argc, char *argv[])
{
    LvglPlus::initialize();
    addSdlPeripherals(300, 400);
    auto root = std::make_shared<Text>("Hello, world!");
    LvglPlus::setScreen(root);
    LvglPlus::runApp();
    return 0;
}

```

What you have to do to build your first interface:

* Include `"lvgl_plus.hh"` - this file provides you `LvglPlus` class
* Include `"widgets/text.hh"` - this file provides you `Text` widget class
* Call `initialize()` method to initialize LVGL and prepare LvglPlus.
* Set drivers for display and input devices with `addSdlPeripherals`.
* Create at least one widget which will be a root widget.
* Feed this root widget to `setScreen` function.
* Start user interface drawing by calling `runApp()`.

After calling `runApp`, your program will get into an infinite loop, which will be exited only after `stopApp` function.
This loop will process gesture, redraw and other lvgl events.

User should be able to create a lot of widgets, but only active tree should be drawn. Other trees will store whole
information about _how and what_ to draw, but don't draw anything until they are not connected to the root tree.
Otherwise, if some widget tree was disconnected (disposed, replaced, deleted), it should store all params anyway,
because it might be connected and drawn again.

### A declaration is not a construct <a name="building-is-not-drawing"></a>

When you wrote

```c++
auto myWidget = std::make_shared<Container>();
```

it doesn't mean that on this line widget will be drawn. It just means that
a Container widget with such parameters might be drawn in the future.
It's just a declaration.
You can do some work between widget declarations, let's expand this example:

```c++
    auto myWidget = std::make_shared<Container>();
    doSomeWork();
    myWidget->addChild(std::make_shared<Text>("Some text");
```

It means that when this widget is drawn, a Text widget will be added to it as a child.

### Apply parameters <a name="apply-parameters"></a>

To apply parameters to a widget, you can use ParamsBuilder class (for each widget you can use specific builder,
e.g. TextParamsBuilder for Text) and pass different parameters to it (for example: width, height, border width,
background color, opacity, etc.).

Applying parameters on the spot (to Text in this example):

```c++
auto myWidget = std::make_shared<Text>("Some text", TextParamsBuilder()
                                                        .width(100)
                                                        .height(50)
                                                        .borderWidth(2));
```

Also, here are special parameters-classes:

* Align
* Padding
* Offset

#### Align <a name="align-param"></a>

Adding alignment to a widget in relation to the parent widget.

```c++
root->addChild(std::make_shared<Text>("Some text", TextParamsBuilder().alignment(Alignment::center));
```

#### Padding <a name="padding-param"></a>

You can adjust the paddings for all widgets (It makes the content area smaller in the chosen
direction): `top`, `right`, `bottom`, `left`.
You can also set additional padding for widgets with children (padding between children): `row` (vertical), `column` (
horizontal).
To set padding for a widget, you need to pass a `Padding`-type object as an attribute to `addChild` method.

Setting padding with constructor (top=1, right=2, bottom=3, left=4):

```c++
root->addChild(std::make_shared<Row>(RowParamsBuilder().padding(Padding(1,2,3,4)));
```

Setting layout widget with constructor (top=1, right=2, bottom=3, left=4, row=5, column=6):

```c++
root->addChild(std::make_shared<Row>(RowParamsBuilder().padding(Padding(1,2,3,4,5,6)));
```

Setting padding with setters (top=1, left=2):

```c++
root->addChild(std::make_shared<Row>(RowParamsBuilder().padding(Padding().top(1).left(2)));
```

#### Offset <a name="offset-param"></a>

Place widget with given offset relative to the parent. Accepts `Point` param, which means `x offset` and
`y offset`. Format:

```c++
root->addChild(std::make_shared<Container>(ContainerParamsBuilder().offset(Offset({50, 100})));
```

### User-defined widget creation <a name="user-defined-widget-creation"></a>

You can create your own widget using base widgets (Container, Column, Text and so on). All you need is inherit
`Container` (or `Column`/`Row` to allow adding more than 1 widget) and define a widget tree in its constructor. This
way you can create not only simple widget, also a whole screens, also using your own previously created widgets.

Let's see an example of simple widget:

```c++
class MySimpleWidget
    : public Container
{
public:
    MySimpleWidget()
    {
        // clang-format off
        auto column = addChild(std::make_shared<Column>());
            column->addChild(std::make_shared<Text>("Header text", TextParamsBuilder().alignment(Alignment::center)));
            auto row = column -> addChild(std::make_shared<Row>());
                row->addChild(std::make_shared<Text>("Left text under the header"));
                row->addChild(std::make_shared<Text>("Right text under the header"));
        // clang-format on
    }
};
```

Here we'll create a widget, which contains column of text and row, which contains 2 texts.
To do this work, we just added a widget tree into Constructor. And that's all!

It's not bad idea to turn off clang-format to place widgets visually at different levels. This helps to see the
structure of widgets on the screen without rendering already at the stage of coding.

### Interaction with widgets <a name="interaction-with-widgets"></a>

LVGL Plus provides two ways to interact with widgets. Of course, you can use other ways, but those two should be
simpler.

#### Widget controller <a name="control-by-controller"></a>

This way is very common for QT users. Some widgets (maybe [your own](#user-defined-widget-creation)) provides
a controller. Once you have controller you can pass it to your backend and get/set widget state.
For example, `TextController` provides you control levers to widgets where you can set and get string content.

```c++
auto text = std::make_shared<Text>("Old text");
auto textController = text->controller;
// ... some code
textController.getText();
textController.text("new text");
```

#### Stream <a name="control-by-stream"></a>

This way represents reactive approach. Streams allow you to set a data trap long before first data arrivals.
It might be very useful in case if you want to rebuild some widget on some events. It's very common
way to implement state-management patterns (like [BLoC](https://bloclibrary.dev/)). For example, you can set a trap on
a list of events and rebuild whole screen according to them. It's highly recommended to take a look at `Heater example`
from examples folder.
Anyway, let's see simple example:

```c++
#include "lvgl_plus.hh"
#include "navigation.hh"
#include "helpers/sdl/sdl_drv.hh"
#include <communication/streams/include/stream.hh>
#include <communication/streams/include/stream_controller.hh>
#include <communication/streams/include/stream_event_loop.hh>
#include <memory>

using namespace softeq::lvglplus;
using namespace softeq::streams;

int main()
{
    LvglPlus::initialize();
    addSdlPeripherals(300, 400);
    streamEventLoop = std::make_shared<StreamEventLoop>();
    auto navigationController = std::make_shared<StreamController<NavigationState>>(streamEventLoop);
    auto root = std::make_shared<Container>();
    auto navigationSubscription = navigationController->stream()->listen(
        [&](auto page)
        {
            switch (page)
            {
            case NavigationState::MainPage:
            {
                auto screen = std::make_shared<FirstPage>(navigationController);
                root->replaceChild(screen);
                break;
            }
            case NavigationState::SettingsPage:
            {
                auto screen = std::make_shared<SecondPage>(navigationController);
                root->replaceChild(screen);
                break;
            }
            }
        });
    root->addChild(std::make_shared<FirstPage>(navigationController));
    LvglPlus::setScreen(root);
    LvglPlus::runApp();
}
```

Here we have two pages. Both of them accept navigationController (Stream object), into which they can push events:

```c++
navigationController->add(NavigationState::SettingsPage);
```

Once event has been emitted, after some short time (you don't really know when, because Streams uses event loop)
your trap will receive new event and replace child for the root widget. It could be done again and again.

## Concept description <a name="concept-description"></a>

### Inheritance tree <a name="inheritance-tree"></a>

#### Widget <a name="widget-class"></a>

Widget is a base class for all widgets in LVGL Plus library.
This class provides possibility to trace widgets tree and debug interface by `toString()` method. This method returns
object `type name` and, if any name has been set by `setDiagnosticName()`, additionally concat to result this name.
For example:

```c++
auto myWidget = std::make_shared<Container>();
myWidget->setDiagnosticName("my container");
std::cout << myWidget->toString() << std::endl;
```

writes to stdout:

```
Instance of Container (my container)
```

Widgets are **thread-safe** objects: when `runApp()` is calling,
it's creating mutex for `lv_...` calls and inject it into the passed root widget. All children of this root
will use this mutex even they were added after running `runApp()`.
It archives by lazy initialization of `lvglMutex` field.
Also, each widget has its ows mutex to protect internal structure.

Widget exposes this public interface:

* renderWithChildren() - **pure virtual method** which initiates drawing itself and all children.
* render() - **pure virtual method** which calls right after `renderWithChildren()` and does widget-specific
  operations.
  Should be defined in each material (user-defined) widget.
* getLvObject() - returns native pointer to stored LVGL object.

#### Children container <a name="children-container"></a>

Every `Widget` has possibility to contain children. To store children, `Widget` has children container that can provide
following behavior:
`noChild` - children can't be added.
`singleChild` - only one child can be added.
`multiChild` - no limit of the children quantity.

To set the container behavior you can use following syntax:
`_children = WidgetContainer::MakeWidgetContainer(WidgetContainerType::singleChild);`

Methods of manipulating children:

* `addChild()` - add child to this widget. `noChild` will ignore it, `singleChild` will replace old child if
  exists, `multiChild` will add new widget along with others.
* `replaceChild()` - remove existing child, add passed widget as a new child and render it.
* `noChild` will ignore it, `singleChild` will replace old child if
  exists, `multiChild` will replace all children.

Because this type of widgets has a lot of children, there might appear alignment difficulties, so
`MultiChildWidget` have some public functions, but they need only for internal usage:

* `getAlignToLvObject()` - returns native LVGL pointer to the last child from children list. It's needed to pass it to
  the render functions to handle correct drawing order.
* `setLastChild()` - this functions calls every time when drawable widget (for example, StatelessWidget) has been drawn.

#### Debug widget tree <a name="debug-widget-tree"></a>

`Widget` provides debugging tools like `wholeTreeToString` and `subtreeToString` that return string with visual
representation of a widget tree (using `toString()` method to "draw" widget).

`wholeTreeToString()` method returns string visual representation of the whole widget tree (from the root) and mark the
widget that calls this method.

`subtreeToString()` method returns string visual representation of the widget subtree (widget as a root).

Example:

```c++
auto root = std::make_shared<Container>();
auto column = std::make_shared<Column>();
root->addChild(column);
column->addChild(std::make_shared<Text>());
column->addChild(std::make_shared<Text>());
auto container = std::make_shared<Container>();
container->addChild(std::make_shared<Text>());
column->addChild(container);
std::cout << container->wholeTreeToString() << std::endl;
std::cout << container->subtreeToString() << std::endl;
```

In the example simple tree has been created. `container` is a child of `column`, that child of `root`.

`container->wholeTreeToString()` will return visual representation of `root` tree, find `container` and mark it.

`container->subtreeToString()` will return visual representation of `container` tree.

Terminal output:

```c++
Widget tree:
Instance of Container (amount of children - 1):
|  Instance of Column (amount of children - 3):
|  |  Instance of Text
|  |  Instance of Text
|  |  Instance of Container (amount of children - 1): <-------- Marked widget
|  |  |  Instance of Text

Instance of Container (amount of children - 1):
|  Instance of Text
```

### Main loop (runApp) <a name="main-loop-runapp"></a>

The `runApp()` function is the main entry point of whole library. It provides such functionality:

* Creates a mutex for working with LVGL library, because naturally it's **not** thread-safe
* Calls `renderWithChildren()` for the root widget and passes created mutex to it
* Handles UI quit/close/dispose event
* Takes processor time to LVGL calling `lv_task_handler()`
* Sleeps this thread for a few _ms_ to take a processor time to other program modules

_It's very important to do all work with LVGL functions (has `lv_` prefix) under the mutex._

### Basic widget creation <a name="basic-widget-creation"></a>

This process is very similar to [User-defined widget creation](#user-defined-widget-creation), but we have to manipulate
with LVGL objects and, probably, define custom `render()` function.
You can see `Button` widget and inspect its code to understand how to create basic widgets.

### Widget catalog <a name="widget-catalog"></a>

To see all available widgets there is a `widget catalog` in the `examples` directory.
The widgets are placed in a column to show what they look like and how to interact with them.
Here is a list of the widgets:

- `Container`: Layout widget. No auto align, contains one child by default.
- `Row`: Layout widget. Every new child widget will be added to the bottom of the previous one.
- `Column`: Layout widget. Every new child widget will be added to the right of the previous one.
- `Text` is used to display text.
- `TextField`: A widget with a Text and a cursor on it. Texts or characters can be added to it.
- `Button` responds on clicks.
- `Checkbox` is created from a "tick box" and a Text. When the Checkbox is clicked the tick box is toggled.
- `Switch` looks like a little slider and can be used to turn something on and off.
- `DropDownButton` allows the user to select one value from a list.
- `LinearProgressIndicator` has a background and an indicator on it. The width of the indicator is set according to the
  current value of the bar.
- `ListWheelScrollView` allows a user to simply select one option from a list by scrolling.
- `Slider` looks like a LinearProgressIndicator supplemented with a knob. The knob can be dragged to set a value.
- `CircularSlider` like a Slider has the knob to set a value but looks like a circle.
- `Divider` used to separate widgets in a row/column with a colored line.

Every widget has a pack of parameters. These parameters can be set by setters or by the parameter builder passed to the
widget constructor.

For example to set the width of the Row use this code:

```c++
    auto row = std::make_shared<Row>();
    row->setWidth(100);
```

Also, the width can be set by RowParamsBuilder:

```c++
    auto row = std::make_shared<Row>(RowParamsBuilder().width(100));
```

Every ParamsBuilder can be created before and be used for different Row widgets:

```c++
    auto rowParamsBuilder = RowParamsBuilder().width(100);
    auto row1 = std::make_shared<Row>(rowParamsBuilder);
    auto row2 = std::make_shared<Row>(rowParamsBuilder);
```

### Stream widget builder <a name="stream-widget-builder"></a>

There is a special widget that can replace its child when new data occures in a stream.

```c++
auto window = std::make_shared<StreamWidgetBuilder<NavigationState>>(
    NavigationState::MainPage, navigationController->stream(),
    [&](const NavigationState &navigationState) -> std::shared_ptr<Widget>
    {
        switch (navigationState)
        {
        case NavigationState::MainPage:
            return std::make_shared<MainPage>(navigationController);
        case NavigationState::SettingsPage:
            return std::make_shared<SettingsPage>(navigationController);
        default:
            return nullptr;
        }
    });
```

Here `window` widget can switch child to `MainPage` or `SettingsPage` when `navigationController->stream()`
catches `NavigationState::MainPage` or `NavigationState::SettingsPage`.

At first, you need to create `StreamWidgetBuilder` with `<NavigationState>`
type of stream data trap. Wrap it to `shared_ptr` to have an ability to add it in a widget tree.

Arguments:

1) Initial value (it will create widget depending on `NavigationState::MainPage`).
2) Stream (`navigationController->stream()`)
3) Builder lambda function that returns a `shared_ptr` of new widget.

Any logic can be implemented in the bulder function, but it is important to return a widget.
In the example, `switch` checks for incoming data and returns a new widget.

Then, you can create button with a click event handler:
`navigationController->add(NavigationState::SettingsPage);`. When the button is pressed `window` will
contain the `SettingsPage` widget.

### BLoC pattern <a name="bloc-pattern"></a>

Like in Flutter, LVGL+ has a BLoC pattern support (simplifyed). UI can raise event, e.g. by pressing a button. BLoC
catches
it, processes it, and generate a state to update UI.

To use BLoC, you need to create some files in a separate folder:
main BLoC logic file, events file, states file.

```
navigation_bloc
--- navigation_bloc.hh
--- navigation_event.hh
--- navigation_state.hh
```

`navigation_event.hh`

For example, you want to switch a page on button click. Button generates a BLoC event to initiate a page switch.

Let's create events. We have 2 pages: `MainPage` and `SettingsPage`. One button can generate event to open `MainPage`,
and another one to open `SettingsPage`. Let's name base event as `NavigationEvent` and inherit it in `OpenMainPageEvent`
and `OpenSettingsPageEvent` (to allow BLoC understand that states are connected).

```c++
class NavigationEvent
{
public:
    virtual ~NavigationEvent() = default;
};

class OpenMainPageEvent : public NavigationEvent {};
class OpenSettingsPageEvent : public NavigationEvent {};
```

Note, that the base class must be abstract and contain a virtual method, e.g. destructor.

`navigation_state.hh`

When the event happend, you can generate a new state for the page. There are 2 possible states (pages):
`MainPage` and `SettingsPage`, let's name states as `NavigationMainPageState` and `NavigationSettingsPageState`
and inherit it from a base state named `NavigationState`.

```c++
class NavigationState
{
public:
    virtual ~NavigationState() = default;
};

class NavigationMainPageState : public NavigationState
{
public:
    std::string title = "Heater device";
};
class NavigationSettingsPageState : public NavigationState
{
public:
    std::string title = "Settings";
};
```

States can contain any data. In the example titles a saved in the page states.

`navigation_bloc.hh`

In the main BLoC file you need to create a class, inherited from the Bloc class of LVGL+.
Bloc is a template class, and you need to tell it what types of events and states it will process
(types of the base classes: `NavigationEvent`, `NavigationState`)

In the constructor you need to tell BLoC `on` which event it should react to and how.

There is an example below.

```c++
class NavigationBloc : public Bloc<NavigationEvent, NavigationState>
{
public:
    NavigationBloc()
        : Bloc<NavigationEvent, NavigationState>(NavigationMainPageState())
    {
        on<OpenMainPageEvent>([this](OpenMainPageEvent event) { this->emit(NavigationMainPageState()); });
        on<OpenSettingsPageEvent>([this](OpenSettingsPageEvent event) { this->emit(NavigationSettingsPageState()); });
    }

private:
};
```

`on<OpenMainPageEvent>` means that BLoC will react on `OpenMainPageEvent`.
An argument of this function is a void lambda function. When the event occurs, it
generates a new state `NavigationMainPageState()` and emits the state (`this->emit`) to the Bloc.

There can be as many event handlers as events declared in `navigation_event.hh`.

#### BLoC widget builder <a name="bloc-widget-builder"></a>

Like `StreamWidgetBuilder`, `BlocWidgetBuilder` can update widget when data was changed. But in this case you
don't need to pass stream and initial state as arguments, just pass shared pointer of your BLoC class.
The initial value is already stored in the user BLoC class.
To identify type of comming state, use `getIf` method of BLoC class.
It returns shared pointer to the state if `state` has the same type as passed to `<>` and returns `nullptr` if not.

```c++
auto window = std::make_shared<BlocWidgetBuilder<NavigationBloc, NavigationState>>(
    navigationBloc,
    [&](const std::shared_ptr<NavigationState> &state) -> std::shared_ptr<Widget>
    {
        if (auto stateMainPage = navigationBloc->getIf<NavigationMainPageState>(state))
        {
            return std::make_shared<MainPage>(navigationBloc, stateMainPage->title);
        }

        if (auto stateSettingsPage = navigationBloc->getIf<NavigationSettingsPageState>(state))
        {
            return std::make_shared<SettingsPage>(navigationBloc, stateSettingsPage->title);
        }

        return nullptr;
    });
```

In the example `state` checked if the type is `NavigationMainPageState` and `MainPage` creates if true.
Also, `state` checked if the type is `NavigationSettingsPageState` and `SettingsPage` creates if true.
State contains data and you can get it. For example, page title was obtained from `state` and passed as an argument to
the page constructor.