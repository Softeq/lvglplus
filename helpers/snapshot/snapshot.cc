#include "snapshot.hh"
#include "lvgl.h"
#include "lvgl_plus.hh"
#include <fstream>
#include <iostream>

BitMap takeSnapshot()
{
    softeq::lvglplus::UniqueLock locker(softeq::lvglplus::LvglPlus::lvglMutex());
    lv_img_dsc_t *snap = lv_snapshot_take(lv_scr_act(), LV_IMG_CF_TRUE_COLOR_ALPHA);
    locker.unlock();

    int width = snap->header.w;
    int height = snap->header.h;
    BitMap image(width, height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int dataPos = (y * width + x) * 4;
            uint8_t r = snap->data[dataPos + 0];
            uint8_t g = snap->data[dataPos + 1];
            uint8_t b = snap->data[dataPos + 2];
            uint8_t a = snap->data[dataPos + 3];
            image.setColor(x, y, Color(r, g, b, a));
        }
    }
    lv_snapshot_free(snap);
    image.generateHeader();
    return image;
}

bool saveSnapshot(const std::string &folderPath, const std::string &sampleName)
{
    auto snapshotImage = takeSnapshot();
    std::string filePath = folderPath + "/" + sampleName + BitMap::fileExtension;
    return snapshotImage.saveImageToFile(filePath, ColorFormat::RGBA);
}

bool compareScreenWithSample(const std::string &inFilePath, const std::string &sampleName, float tolerance,
                             const std::string &outFilePath)
{
    std::string filePath = inFilePath + "/" + sampleName + BitMap::fileExtension;
    // try to open sample
    std::ifstream iff;
    iff.open(filePath);
    if (!iff.is_open())
    {
        std::cerr << "Error: sample \"" + sampleName + "\" not found." << std::endl;
        return false;
    }
    iff.close();
    // prepare images to compare: take snapshot and load sample
    auto snapshotImage = takeSnapshot();
    BitMap exampleImage = BitMap::readImageFromFile(filePath, ColorFormat::RGBA);

    // check "findDifferencesWith" will not find difference
    float diffPercent = 0.0;
    BitMap diffImage = BitMap::markDifferences(snapshotImage, exampleImage, diffPercent);
    if (diffPercent > tolerance)
    {
        if (!outFilePath.empty())
        {
            BitMap glued = BitMap::glueImages(exampleImage, diffImage);
            glued = BitMap::glueImages(glued, snapshotImage);
            glued.saveImageToFile(outFilePath + "/" + sampleName + "_fail" + BitMap::fileExtension, ColorFormat::RGBA);
        }
        return false;
    }
    return true;
}