#pragma once
#include "Image/ImageData.h"

namespace Kaguya
{
namespace TextureSynthesis
{
inline Vector3f ColorToVector(const ColorRGBA &clr)
{
    return Vector3f(clr.r, clr.g, clr.b);

}
ImageData* synth(const ImageData* src, int wdt = 640, int hgt = 480, int nr = 2);
}

}
