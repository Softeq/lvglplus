#ifndef LVGL_PLUS_BITMAP_HH
#define LVGL_PLUS_BITMAP_HH

#include "vector"
#include <cstdint>
#include <string>

struct Color
{
    uint8_t r, g, b, a;
    Color();
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    Color(uint8_t r, uint8_t g, uint8_t b);
    ~Color();
    bool operator==(const Color &otherColor) const;
    bool operator!=(const Color &otherColor) const;
};

struct BmpHeader
{
    uint8_t bitmapSignatureBytes[2] = {'B', 'M'};
    uint8_t fileSize[4] = {0, 0, 0, 0};
    uint8_t reservedBytes[4] = {0, 0, 0, 0};
    uint8_t pixelDataOffset[4] = {0, 0, 0, 0};
};

struct BmpInfoHeader
{
    uint32_t sizeOfThisHeader = 0;
    int32_t width = 0;
    int32_t height = 0;
    uint16_t numberOfColorPlanes = 0;
    uint16_t colorDepth = 0;
    uint32_t compressionMethod = 0;
    uint32_t rawBitmapDataSize = 0;
    uint32_t horizontalResolution = 0;
    uint32_t verticalResolution = 0;
    uint32_t colorTableEntries = 0;
    uint32_t importantColors = 0;
};

enum class ColorFormat
{
    RGBA,
    BGRA
};

class BitMap
{
public:
    BitMap();
    BitMap(int width, int height);
    ~BitMap();

    Color getColor(int x, int y) const;
    void setColor(int x, int y, const Color &color);
    bool saveImageToFile(const std::string &path, ColorFormat colorFormat = ColorFormat::BGRA);
    static BitMap readImageFromFile(const std::string &path, ColorFormat colorFormat = ColorFormat::BGRA);
    int getHeight() const
    {
        return _height;
    }
    int getWidth() const
    {
        return _width;
    }
    void generateHeader();
    static BitMap markDifferences(BitMap &bmp1, BitMap &bmp2, float &diffPercent);
    static BitMap glueImages(BitMap &bmp1, BitMap &bmp2);
    static const std::string fileExtension;
    static bool compareHeaders(BitMap &bmp1, BitMap &bmp2);
    static bool isEqual(BitMap &bmp1, BitMap &bmp2);

private:
    BmpHeader bmpHeader;
    BmpInfoHeader bmpInfoHeader;
    static const uint32_t fileHeaderSize;
    static const uint32_t infoHeaderSize;
    int _width = 0;
    int _height = 0;
    std::vector<Color> _colors;
};

#endif // LVGL_PLUS_BITMAP_HH
