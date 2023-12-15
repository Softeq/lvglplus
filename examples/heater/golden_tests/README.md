# Golden tests <a name="lvgl-plus"></a>

## Index <a name="index"></a>

- [Index](#index)
- [How to use](#how-to-use)
    - [Build](#build)
        - [CMake options](#cmake-options)
        - [Build golden tests example](#build)
    - [Run](#run)
        - [Create a sample](#create-a-sample)
        - [Compare with a sample](#compare-with-a-sample)
        - [Choose a sample handler](#choose-a-sample-handler)
- [Other](#other)
    - [BMP](#bmp)

# How to use <a name="how-to-use"></a>

Golden tests are a type of automated testing used to ensure visual consistency of graphical user interfaces. In this
example, golden tests are implemented using the `snapshot.h` functions to capture a snapshot of the screen and compare
it to a previously saved image.

## Build <a name="build"></a>

### CMake options <a name="cmake-options"></a>

To enable golden tests, run the CMake command with the `BUILD_GOLDEN_TESTS` option set to `ON`:

`cmake -B build -DBUILD_GOLDEN_TESTS=on`

If the flag is set, the samples in your source directory will be copied to your binary directory.

To update the golden test samples, run the CMake command with the `UPDATE_GOLDEN_SAMPLES` option set to `ON`:

`cmake -B build -DUPDATE_GOLDEN_SAMPLES=on`

The new samples can be found in the `your-binary-directory/golden_tests/samples` directory, and can be copied to the
project directory with your other samples.

You can specify the acceptable percentage of mismatched pixels between the original source and the displayed image using
the `ACCEPTABLE_MISMATCH_PERCENTAGE` option:

`cmake -B build -DACCEPTABLE_MISMATCH_PERCENTAGE=0.1`

### Build golden tests example <a name="build"></a>

Open `lvglplus` directory in a terminal and continue with followed commands:

1. `cmake -B build -S . -G "Unix Makefiles"`
2. `cmake --build build --target test_heater_golden`
3. `./build/examples/heater/test_heater_golden`

## Run <a name="run"></a>

### Create a sample <a name="create-a-sample"></a>

To capture a snapshot of the screen and save it to a file, you can use
the `saveSnapshot(*path*, *sampleName*)` function. This will generate a BMP file with the name specified
in `sampleName`, located in the directory specified in `path`.

By default, the directory for the samples is `your-binary-directory/golden_tests/samples` in the example. You can modify
this directory in the `CMakeLists.txt` file.

### Compare with a sample <a name="compare-with-a-sample"></a>

To capture a snapshot of the screen and compare it to a previously saved sample, you can use
the `compareScreenWithSample(*path-to-sample*, *sampleName*, *mismatch*, *path-to-failed*)` function. This
will generate a BMP file for the current snapshot and compare it to the BMP file with the name specified
in `sampleName`, located in the `path-to-sample` directory.

The test will pass if the percentage of mismatched pixels is below the `mismatch` threshold. If there are more
differences between the two BMP files than allowed, the program will generate a new BMP file with the `sampleName` in
the `path-to-failed` directory to indicate that the test has failed.

By default, the directory for failed tests is `your-binary-directory/golden_tests/failed` in the example. You can modify
this directory in the `CMakeLists.txt` file.

### Choose a sample handler <a name="choose-a-sample-handler"></a>

In the given example, there is a function called `checkSnapshot(*sampleName*)`. This function checks the value of the
CMake option `MAKE_GOLDEN_TESTS_SAMPLES`. If the option is set to `ON`, the function creates a new sample with the
specified name. If the option is set to `OFF`, the function compares the snapshot with the saved sample, which was
generated previously when the option was set to `ON`.

## Other <a name="other"></a>

### BMP <a name="bmp"></a>

The BitMap class can be used to generate .bmp files and save them or load and edit them. There is a
`saveImageToFile(*path*)` function to save a BMP file and a `readImageFromFile(*path*)` function to read one.

An important function for implementing golden tests is `markDifferences(*bmp1*, *bmp2*, *diffPercent*)`, which compares
two bitmaps and returns a new bitmap with a mask of the differences in pixels. It also sets `diffPercent` to the
percentage of differing pixels.

Another function to allow visual comparison for the user is `glueImages(*bmp1*, *bmp2*)`, which returns a new bitmap
that contains two received bitmaps in a row.
