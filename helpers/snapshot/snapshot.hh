#ifndef LVGL_PLUS_SNAPSHOT_HH
#define LVGL_PLUS_SNAPSHOT_HH

#include "bitmap.hh"

BitMap takeSnapshot();
bool compareScreenWithSample(const std::string &inFilePath, const std::string &sampleName, float tolerance,
                             const std::string &outFilePath = "");
bool saveSnapshot(const std::string &folderPath, const std::string &sampleName);

#endif // LVGL_PLUS_SNAPSHOT_HH
