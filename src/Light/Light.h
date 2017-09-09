#pragma once
#include "Math/Transform.h"
#include "Light/Spectrum.h"

namespace Kaguya
{

enum class LightFlag : uint8_t
{
	DELTA_POINT = 1,
	DELTA_DIRECTION = 2,
	AREA = 4,
	INFINITE_AREA = 8
};

class Light
{
public:
	Light();
	~Light();

public:
	LightFlag mFlag;
protected:
	Transform mWorldToLight;
};

}
