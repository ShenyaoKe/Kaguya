#pragma once
#ifndef __ENVIRONMENTMAP__
#define __ENVIRONMENTMAP__

//#include "Core/rtdef.h"
#include "Shading/TextureMapping.h"
#include "Shading/Texture.h"

class EnvironmentMap
{
private:
	FileTexture tex;
	SphericalMapping2D mapping;
public:
	EnvironmentMap();
	EnvironmentMap(const string& filename);
	~EnvironmentMap();

	ColorRGBA getColor(const Vector3f &dir) const;
};


#endif // __ENVIRONMENTMAP__