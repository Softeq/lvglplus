#include "../heater.hh"
#include "sdl_drv.hh"
#include "snapshot.hh"
#include "wait_for.hh"
#include <gtest/gtest.h>

using namespace softeq::lvglplus;

static bool checkSnapshot(const std::string &sampleName)
{
    // make new samples if MAKE_GOLDEN_TESTS_SAMPLES is on or compare the snapshot with the existing sample
#ifdef MAKE_GOLDEN_TESTS_SAMPLES
    return saveSnapshot(GOLDEN_SAMPLES_BINARY_DIR, sampleName);
#else
    return compareScreenWithSample(GOLDEN_SAMPLES_BINARY_DIR, sampleName, ACCEPTABLE_MISMATCH_PERCENTAGE,
                                   GOLDEN_FAILED_BINARY_DIR);
#endif
}

static bool sdlInitialized = false;

TEST(HeaterGoldenTest, MainPageTest)
{
    LvglPlus::initialize();
    if (!sdlInitialized)
    {
        addSdlPeripherals(LVGL_SCREEN_WIDTH, LVGL_SCREEN_HEIGHT);
        sdlInitialized = true;
    }
    Heater heater;
    LvglPlus::setScreen(heater.root); // run lvgl plus to render heater
    LvglPlus::runApp(true);
    // check if start page looks like main page
    const std::string sampleName = "main_page_sample";
    ASSERT_TRUE(checkSnapshot(sampleName));
}

TEST(HeaterGoldenTest, SettingsPageTest)
{
    LvglPlus::initialize();
    if (!sdlInitialized)
    {
        addSdlPeripherals(LVGL_SCREEN_WIDTH, LVGL_SCREEN_HEIGHT);
        sdlInitialized = true;
    }
    Heater heater;
    bool equalFlag = false;
    // check if after changing page the result looks like settings page
    std::thread doActions([&]() {
        ASSERT_TRUE(waitForRender(heater.root));
        auto snap = takeSnapshot();
        heater.navigationBloc->add(OpenSettingsPageEvent());
        // wait until stream process end
        BitMap newSnap;
        ASSERT_TRUE(equalFlag = waitForCondition([&]() {
                        newSnap = takeSnapshot();
                        return !(BitMap::isEqual(snap, newSnap));
                    }));
        const std::string sampleName = "settings_page_sample";
        equalFlag = checkSnapshot(sampleName);
        LvglPlus::stopApp();
    });
    LvglPlus::setScreen(heater.root); // run lvgl plus to render heater
    LvglPlus::runApp();
    doActions.join();
    ASSERT_TRUE(equalFlag);
}