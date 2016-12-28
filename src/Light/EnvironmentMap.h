#pragma once

//#include "Core/rtdef.h"
#include "Shading/TextureMapping.h"
#include "Shading/Texture.h"

namespace Kaguya
{

class EnvironmentMap
{
private:
    FileTexture tex;
    SphericalMapping2D mapping;
public:
    EnvironmentMap();
    EnvironmentMap(const std::string &filename);
    ~EnvironmentMap();

    ColorRGBA getColor(const Vector3f &dir) const;
};

}
