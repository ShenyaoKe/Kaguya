#pragma once

#include "Image/ImageData.h"
#include "Image/Filter.h"

namespace Kaguya
{

class ImagePyramid
{
    int levels;
    vector<ImageData*> layers;
public:
    ImagePyramid(const ImageData &src, int lv = 3);
    ~ImagePyramid();

    void getPixels(int &wdt, int &hgt, unsigned char* &pixMap) const;
};

}
