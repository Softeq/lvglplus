#include "bitmap.hh"
#include <fstream>
#include <iostream>

const uint32_t BitMap::fileHeaderSize = 14;
const uint32_t BitMap::infoHeaderSize = 40;
const std::string BitMap::fileExtension = ".bmp";

Color::Color()
    : r(0)
    , g(0)
    , b(0)
    , a(255)
{
}

Color::Color(uint8_t r, uint8_t g, uint8_t b)
    : r(r)
    , g(g)
    , b(b)
    , a(255)
{
}

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : r(r)
    , g(g)
    , b(b)
    , a(a)
{
}

bool Color::operator!=(const Color &otherColor) const
{
    return !(*this == otherColor);
}

bool Color::operator==(const Color &otherColor) const
{
    if ((r != otherColor.r) || (g != otherColor.g) || (b != otherColor.b) || (a != otherColor.a))
    {
        return false;
    }
    else
    {
        return true;
    }
}

Color::~Color() = default;

BitMap::BitMap(int width, int height)
    : _width(width)
    , _height(height)
    , _colors(std::vector<Color>(width * height))
{
}
BitMap::BitMap()
    : BitMap(0, 0)
{
}

BitMap::~BitMap() = default;

Color BitMap::getColor(int x, int y) const
{
    return _colors[y * _width + x];
}

void BitMap::setColor(int x, int y, const Color &color)
{
    _colors[y * _width + x].r = color.r;
    _colors[y * _width + x].g = color.g;
    _colors[y * _width + x].b = color.b;
    _colors[y * _width + x].a = color.a;
}

BitMap BitMap::readImageFromFile(const std::string &path, ColorFormat colorFormat)
{
    std::ifstream inFile;
    inFile.open(path, std::ios::in | std::ios::binary);
    if (!inFile.is_open())
    {
        return {0, 0};
    }

    BmpHeader readBmpHeader;
    BmpInfoHeader readBmpInfoHeader;
    inFile.read(reinterpret_cast<char *>(&readBmpHeader), fileHeaderSize);
    inFile.read(reinterpret_cast<char *>(&readBmpInfoHeader), infoHeaderSize);
    BitMap image(readBmpInfoHeader.width, readBmpInfoHeader.height);
    image.bmpHeader = readBmpHeader;
    image.bmpInfoHeader = readBmpInfoHeader;

    Color pixelColor;
    for (int y = image._height - 1; y >= 0; y--)
    {
        for (int x = 0; x < image._width; x++)
        {
            if (colorFormat == ColorFormat::BGRA)
            {
                inFile.read(reinterpret_cast<char *>(&(pixelColor.b)), 1);
                inFile.read(reinterpret_cast<char *>(&(pixelColor.g)), 1);
                inFile.read(reinterpret_cast<char *>(&(pixelColor.r)), 1);
            }
            else
            {
                inFile.read(reinterpret_cast<char *>(&(pixelColor.r)), 1);
                inFile.read(reinterpret_cast<char *>(&(pixelColor.g)), 1);
                inFile.read(reinterpret_cast<char *>(&(pixelColor.b)), 1);
            }
            inFile.read(reinterpret_cast<char *>(&(pixelColor.a)), 1);
            image.setColor(x, y, pixelColor);
        }
    }
    inFile.close();
    return image;
}

void BitMap::generateHeader()
{
    const uint32_t fileSize = fileHeaderSize + infoHeaderSize + (_width * _height * 4);

    *(reinterpret_cast<uint32_t *>(&bmpHeader.fileSize)) = fileSize;
    *(reinterpret_cast<uint32_t *>(&bmpHeader.pixelDataOffset)) = fileHeaderSize + infoHeaderSize;

    bmpInfoHeader.sizeOfThisHeader = infoHeaderSize;
    bmpInfoHeader.width = _width;
    bmpInfoHeader.height = _height;
    bmpInfoHeader.numberOfColorPlanes = 1;
    bmpInfoHeader.colorDepth = 32;
}

bool BitMap::saveImageToFile(const std::string &path, ColorFormat colorFormat)
{
    std::ofstream outFile;
    outFile.open(path, std::ios::out | std::ios::binary);
    if (!outFile.is_open())
    {
        return false;
    }

    generateHeader();

    outFile.write(reinterpret_cast<char *>(&bmpHeader), fileHeaderSize);
    outFile.write(reinterpret_cast<char *>(&bmpInfoHeader), infoHeaderSize);

    // writing pixel data
    for (int y = _height - 1; y >= 0; y--)
    {
        for (int x = 0; x < _width; x++)
        {
            unsigned char b = static_cast<unsigned char>(getColor(x, y).b);
            unsigned char g = static_cast<unsigned char>(getColor(x, y).g);
            unsigned char r = static_cast<unsigned char>(getColor(x, y).r);
            unsigned char a = static_cast<unsigned char>(getColor(x, y).a);
            unsigned char color[4];
            if (colorFormat == ColorFormat::BGRA)
            {
                color[0] = b;
                color[1] = g;
                color[2] = r;
            }
            else
            {
                color[0] = r;
                color[1] = g;
                color[2] = b;
            }
            color[3] = a;
            outFile.write(reinterpret_cast<char *>(color), 4);
        }
    }
    outFile.close();
    return true;
}

bool BitMap::compareHeaders(BitMap &bmp1, BitMap &bmp2)
{
    if ((bmp2._width != bmp1._width) || (bmp2._height != bmp1._height))
    {
        return false;
    }
    // compare headers
    for (uint32_t i = 0; i < fileHeaderSize; i++)
    {
        if (*reinterpret_cast<char *>(&bmp2.bmpHeader) != *reinterpret_cast<char *>(&bmp1.bmpHeader))
        {
            return false;
        }
    }
    for (uint32_t i = 0; i < infoHeaderSize; i++)
    {
        if (*reinterpret_cast<char *>(&bmp2.bmpInfoHeader) != *reinterpret_cast<char *>(&bmp1.bmpInfoHeader))
        {
            return false;
        }
    }
    return true;
}

bool BitMap::isEqual(BitMap &bmp1, BitMap &bmp2)
{
    if (!compareHeaders(bmp1, bmp2))
    {
        return false;
    }
    for (int i = 0; i < bmp1._width * bmp1._height; i++)
    {
        if (bmp1._colors[i] != bmp2._colors[i])
        {
            return false;
        }
    }
    return true;
}

BitMap BitMap::markDifferences(BitMap &bmp1, BitMap &bmp2, float &diffPercent)
{
    diffPercent = 0.0;
    compareHeaders(bmp1, bmp2);
    auto _width = bmp1._width;
    auto _height = bmp1._height;
    BitMap bmpDiff(_width, _height);
    bmpDiff.generateHeader();
    // writing pixel data
    int diffPixels = 0;
    for (int y = bmp1._height - 1; y >= 0; y--)
    {
        for (int x = 0; x < _width; x++)
        {
            if (bmp1.getColor(x, y).r != bmp2.getColor(x, y).r || bmp1.getColor(x, y).g != bmp2.getColor(x, y).g ||
                bmp1.getColor(x, y).b != bmp2.getColor(x, y).b || bmp1.getColor(x, y).a != bmp2.getColor(x, y).a)
            {
                bmpDiff.setColor(x, y, Color(0, 0, 255));
                diffPixels++;
            }
            else
            {
                uint8_t averageColor = (bmp2.getColor(x, y).r + bmp2.getColor(x, y).g + bmp2.getColor(x, y).b) / 3;
                bmpDiff.setColor(x, y, Color(averageColor, averageColor, averageColor));
            }
        }
    }
    diffPercent =
        (float)100.0 * static_cast<float>(diffPixels) / (static_cast<float>(_width) * static_cast<float>(_height));

    return bmpDiff;
}

BitMap BitMap::glueImages(BitMap &bmp1, BitMap &bmp2)
{
    BitMap newBmp(bmp1._width + bmp2._width + 1, bmp1._height);
    newBmp.generateHeader();

    for (int y = 0; y < newBmp._height; y++)
    {
        for (int x = 0; x < newBmp._width; x++)
        {
            Color color;
            if (x < bmp1._width)
            {
                color = bmp1.getColor(x, y);
            }
            else
            {
                if (x == bmp1._width)
                {
                    color = Color();
                }
                else
                {
                    color = bmp2.getColor(x - bmp1._width - 1, y);
                }
            }
            newBmp.setColor(x, y, color);
        }
    }
    return newBmp;
}
