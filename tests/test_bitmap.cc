#include "bitmap.hh"
#include <gtest/gtest.h>

// Testing if the image data is correct by comparing it with the original image
TEST(ImageTest, SaveAndReadBmpFile)
{
    const int testSize = 10;
    BitMap exampleImage(testSize, testSize);
    // set pixels to random colors
    for (int y = 0; y < exampleImage.getHeight(); y++)
    {
        for (int x = 0; x < exampleImage.getWidth(); x++)
        {
            exampleImage.setColor(x, y, Color(std::rand(), std::rand(), std::rand()));
        }
    }
    std::string path = "test.bmp"; // temporary test file
    exampleImage.saveImageToFile(path);
    BitMap testImage = BitMap::readImageFromFile(path);
    remove(path.c_str()); // remove temporary test file

    // check "findDifferencesWith" will not find difference
    float diffPercent = 0.0;
    BitMap::markDifferences(exampleImage, testImage, diffPercent);
    ASSERT_EQ(diffPercent, 0.0);

    // set pixel color to check "findDifferencesWith" will find difference
    exampleImage.setColor(0, 0, Color(255, 255, 0));
    BitMap::markDifferences(exampleImage, testImage, diffPercent);
    ASSERT_NE(diffPercent, 0.0);
}