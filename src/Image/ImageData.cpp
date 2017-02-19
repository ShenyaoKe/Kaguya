#include "Image/ImageData.h"

namespace Kaguya
{

ImageData::ImageData(uint32_t wd, uint32_t ht)
    : mWidth(wd), mHeight(ht), mBPP(24)
    , mPixels(mWidth, mHeight)
{
}
ImageData::ImageData(uint32_t wd, uint32_t ht, Float* &pixMap)
    : mWidth(wd), mHeight(ht), mBPP(24)
    , mPixels(mWidth, mHeight)
{
    // Declare width and height
    mWidth = wd;
    mHeight = ht;
    mBPP = 24;

    for (size_t i = 0; i < mWidth * mHeight; ++i)
    {
        mPixels[0][i] = ColorRGBA(pixMap[i * 3], pixMap[i * 3 + 1], pixMap[i * 3 + 2]);
    }
}
ImageData::ImageData(uint32_t wd, uint32_t ht, unsigned char* pixMap, uint8_t pixtype)
    : mWidth(wd), mHeight(ht), mBPP(24)
    , mPixels(mWidth, mHeight)
{
    Float inv255 = 1.0 / 255.0f;
    for (uint32_t i = 0; i < mWidth * mHeight; ++i)
    {
        switch (pixtype)
        {
        case RGB:
        {
            mPixels[0][i] = ColorRGBA(pixMap[i * 3] * inv255,
                                      pixMap[i * 3 + 1] * inv255,
                                      pixMap[i * 3 + 2] * inv255);
            break;
        }
        case RGBA:
        {
            mPixels[0][i] = ColorRGBA(pixMap[i * 3] * inv255,
                                      pixMap[i * 3 + 1] * inv255,
                                      pixMap[i * 3 + 2] * inv255,
                                      pixMap[i * 3 + 3] * inv255);
            break;
        }
        case BGRA:
        {
            mPixels[0][i] = ColorRGBA(pixMap[i * 3 + 2] * inv255,
                                      pixMap[i * 3 + 1] * inv255,
                                      pixMap[i * 3] * inv255,
                                      pixMap[i * 3 + 3] * inv255);
            break;
        }
        default:
            break;
        }

    }
}
ImageData::ImageData(const std::string &filename)
{
    // Define image file format
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    // Define a pointer to the image file
    FIBITMAP *dib(0);
    // Define a pointer to the pixel data
    BYTE* bits(0);
    // Define image data type, e.g. RGB16, UINT16, DOUBLE, etc.
    int imgType(0);

    // Check the file signature and deduce its format
    fif = FreeImage_GetFileType(filename.c_str(), 0);
    if (fif == FIF_UNKNOWN)
    {
        fif = FreeImage_GetFIFFromFilename(filename.c_str());
    }
    if (fif == FIF_UNKNOWN)
    {
        std::cout << "ERROR: Unknown image format!" << std::endl;
        abort();
    }

    // Read image data from file
    if (FreeImage_FIFSupportsReading(fif))
    {
        dib = FreeImage_Load(fif, filename.c_str());
    }
    if (!dib)
    {
        std::cout << "ERROR: Unable to read file " << filename << std::endl;
        abort();
    }

    //get image data from dib
    bits = FreeImage_GetBits(dib);
    mWidth = FreeImage_GetWidth(dib);
    mHeight = FreeImage_GetHeight(dib);
    imgType = FreeImage_GetImageType(dib);
    mBPP = FreeImage_GetBPP(dib);

    //if color data exists, initialize image
    if (bits == 0 || mWidth == 0 || mHeight == 0)
    {
        std::cout << "ERROR: Image file contains no available image data!" << std::endl;
        abort();
        //return false;
    }
    else
    {
        mPixels.allocate(mHeight, mWidth);
    }

    // color order is BGR
    Float inv255 = 1.0 / 255.0;
    Float inv65535 = 1.0 / 65535.0;
    switch (imgType)
    {
    case FIT_UNKNOWN:// unknown type
    {
        std::cout << "Cannot recognize image type!" << std::endl;
        break;
    }
    case FIT_BITMAP:// standard image			: 1-, 4-, 8-, 16-, 24-, 32-bit
    {
        BYTE* pixels = bits;
        switch (mBPP)
        {
        case 1:
        {
            for (size_t j = 0; j < mHeight; j++)
            {
                for (size_t i = 0; i < mWidth; i++)
                {
                    size_t idx = (i + mWidth * j) * 3;

                    //std::cout << idx << ":" << pixels[idx] + 0 << std::endl;
                    setRGBA(i, j, ColorRGBA(pixels[idx + 2] * inv255,
                                            pixels[idx + 1] * inv255,
                                            pixels[idx] * inv255));
                }
            }
            break;
        }
        case 4:
        case 8:
        case 16:
        case 24:// 8bit RGB image
        {
            for (size_t j = 0; j < mHeight; j++)
            {
                for (size_t i = 0; i < mWidth; i++)
                {
                    size_t idx = (i + mWidth * j) * 3;

                    //std::cout << idx << ":" << pixels[idx] + 0 << std::endl;
                    setRGBA(i, j, ColorRGBA(pixels[idx + 2] * inv255,
                                            pixels[idx + 1] * inv255,
                                            pixels[idx] * inv255));
                }
            }
            break;
        }
        case 32:// 8bit RGBA image
        {
            for (size_t j = 0; j < mHeight; j++)
            {
                for (size_t i = 0; i < mWidth; i++)
                {
                    size_t idx = (i + mWidth * j) * 4;
                    setRGBA(i, j, ColorRGBA(pixels[idx + 2] * inv255,
                                            pixels[idx + 1] * inv255,
                                            pixels[idx] * inv255,
                                            pixels[idx + 3] * inv255));
                }
            }
            break;
        }
        default:
            break;
        }
        break;
    }
    case FIT_UINT16:// array of unsigned short	: unsigned 16-bit
    {
        break;
    }
    case FIT_INT16:// array of short			: signed 16-bit
    {
        break;
    }
    case FIT_UINT32:// array of unsigned long	: unsigned 32-bit
    {
        break;
    }
    case FIT_INT32:// array of long				: signed 32-bit
    {
        break;
    }
    case FIT_FLOAT:// array of Float			: 32-bit IEEE doubleing point
    {
        break;
    }
    case FIT_DOUBLE:// array of Float			: 64-bit IEEE doubleing point
    {
        break;
    }
    case FIT_COMPLEX:// array of FICOMPLEX		: 2 x 64-bit IEEE doubleing point
    {
        break;
    }
    case FIT_RGB16:// 48-bit RGB image			: 3 x 16-bit
    {
        WORD* pixels = (WORD*)bits;

        for (size_t j = 0; j < mHeight; j++)
        {
            for (size_t i = 0; i < mWidth; i++)
            {
                size_t idx = (i + mWidth * j) * 3;
                setRGBA(i, j, ColorRGBA(pixels[idx + 2] * inv65535,
                                        pixels[idx + 1] * inv65535,
                                        pixels[idx] * inv65535));
            }
            std::cout << std::endl;
        }
        break;
    }
    case FIT_RGBA16:// 64-bit RGBA image		: 4 x 16-bit
    {
        WORD* pixels = (WORD*)bits;
        //std::cout << "num: " << depth / sizeof(pixels[0]) / 8 << std::endl;
        for (size_t j = 0; j < mHeight; j++)
        {
            for (size_t i = 0; i < mWidth; i++)
            {
                size_t idx = (i + mWidth * j) * 3;
                setRGBA(i, j, ColorRGBA(pixels[idx + 2] * inv65535,
                                        pixels[idx + 1] * inv65535,
                                        pixels[idx] * inv65535,
                                        pixels[idx + 3] * inv65535));
            }
        }
        break;
    }
    default:
        break;
    }

    if (dib)
    {
        FreeImage_Unload(dib);
    }
}

ImageData::ImageData(const ImageData &src)
{
    // Declare width and height
    mWidth = src.getWidth();
    mHeight = src.getHeight();

    mPixels.allocate(mHeight, mWidth);

    mPixels.cloneData(src.mPixels);
}

ImageData::~ImageData()
{
}
void ImageData::setRGBA(uint32_t x, uint32_t y, const ColorRGBA &color)
{
    mPixels[y][x] = color;
}
const ColorRGBA &ImageData::getRGBA(uint32_t x, uint32_t y) const
{
    return mPixels[y][x];
}

const ColorRGBA &ImageData::getRGBA(uint32_t idx) const
{
    return mPixels[0][idx];
}
// RGB
void ImageData::getPixels(unsigned char* &pixMap) const
{
    delete[] pixMap;

    pixMap = new unsigned char[mWidth * mHeight * 3];

    for (size_t i = 0; i < mWidth * mHeight; ++i)
    {
        size_t index = i * 3;//(i % height * width + (i / height)) * 3;
        pixMap[index++] = static_cast<uint8_t>(mPixels[0][i].r * 255);
        pixMap[index++] = static_cast<uint8_t>(mPixels[0][i].g * 255);
        pixMap[index] = static_cast<uint8_t>(mPixels[0][i].b * 255);
    }
}

void ImageData::getPixelsRGBA(unsigned char* &pixMap) const
{
    delete[] pixMap;

    pixMap = new unsigned char[mWidth * mHeight * 4];

    for (size_t i = 0; i < mWidth * mHeight; ++i)
    {
        size_t index = i << 2;// (i % height * width + (i / height)) * 4;
        pixMap[index++] = static_cast<uint8_t>(mPixels[0][i].r * 255);
        pixMap[index++] = static_cast<uint8_t>(mPixels[0][i].g * 255);
        pixMap[index++] = static_cast<uint8_t>(mPixels[0][i].b * 255);
        pixMap[index] = static_cast<uint8_t>(mPixels[0][i].a * 255);
    }
}

void ImageData::printRGBA(uint32_t x, uint32_t y) const
{
    std::cout << mPixels[y][x] << std::endl;
}
void ImageData::resize(uint32_t x, uint32_t y)
{
    mWidth = x;
    mHeight = y;

    mPixels.allocate(mHeight, mWidth);
}
ColorRGBA ImageData::bilinearPixel(Float x, Float y) const
{
    while (x < 0)
    {
        x += mWidth;
    }
    while (x >= mWidth)
    {
        x -= mWidth;
    }
    while (y < 0)
    {
        y += mHeight;
    }
    while (y >= mHeight)
    {
        y -= mHeight;
    }
    ColorRGBA pixC[4];
    //std::cout << "x:" << x << "\ty:" << y << std::endl;
    size_t floorX = floorToInt(x), floorY = floorToInt(y);
    Float tX = x - floorX, tY = y - floorY;
    //int ceilX = (tX == 0 || floorX == width - 1) ? floorX : (floorX + 1);
    //int ceilY = (tY == 0 || floorY == height - 1) ? floorY : (floorY + 1);
    size_t ceilX = (floorX == mWidth - 1) ? 0 : (floorX + 1);
    size_t ceilY = (floorY == mHeight - 1) ? 0 : (floorY + 1);
    pixC[0] = getRGBA(floorX, floorY);
    pixC[2] = getRGBA(floorX, ceilY);
    pixC[1] = getRGBA(ceilX, floorY);
    pixC[3] = getRGBA(ceilX, ceilY);

    return lerp(lerp(pixC[0], pixC[1], tX), lerp(pixC[2], pixC[3], tX), tY);
}

bool ImageData::writeFile(const std::string &filename) const
{
    int BBP = 24;
    FIBITMAP* bitmap = FreeImage_Allocate(mWidth, mHeight, BBP);
    RGBQUAD color;

    if (!bitmap)
    {
        std::cout << "Failed to allocate FreeImage data!" << std::endl;
        return false;
    }


    for (size_t j = 0; j < mHeight; j++)
    {
        for (size_t i = 0; i < mWidth; i++)
        {
            //Set color from image data.
            //......
            ColorRGBA curColor = getRGBA(i, j);
            color.rgbRed = static_cast<BYTE>(curColor.r * 255);
            color.rgbGreen = static_cast<BYTE>(curColor.g * 255);
            color.rgbBlue = static_cast<BYTE>(curColor.b * 255);
            FreeImage_SetPixelColor(bitmap, i, j, &color);
        }
    }
    //save image file.
    if (FreeImage_Save(FIF_PNG, bitmap, filename.c_str()))
    {
        std::cout << "Image successfully saved!" << std::endl;
        return true;
    }
    return false;
}

AlignedArray2D<uint32_t>* ImageData::genHist() const
{
    /*int** histRGB = new int*[4];
    int bitlen = 1 << bpp;
    for (size_t i = 0; i < 4; i++)
    {
        histRGB[i] = new int[bitlen];
    }
    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            histRGB[0][static_cast<int>(pixels[i][j]->r)]++;
            histRGB[1][static_cast<int>(pixels[i][j]->r)]++;
            histRGB[2][static_cast<int>(pixels[i][j]->r)]++;
            histRGB[3][static_cast<int>(pixels[i][j]->r)]++;
        }
    }*/
    return nullptr;
}

AlignedArray2D<Float>* ImageData::getLuma() const
{
    AlignedArray2D<Float>* ret = new AlignedArray2D<Float>(mHeight, mWidth);

    for (size_t j = 0; j < mHeight; j++)
    {
        for (size_t i = 0; i < mWidth; i++)
        {
            const ColorRGBA &curColor = mPixels[j][i];
            *ret[j][i] = curColor.r * 0.2126 + curColor.g * 0.7152 + curColor.b * 0.0722;
        }
    }

    return ret;
}

}
