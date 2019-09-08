#pragma once

#include "Math/Transform.h"
#include "Light/Spectrum.h"
#include "Geometry/Intersection.h"

namespace Kaguya
{

enum class LightFlag : uint8_t
{
	DELTA_POSITION = 1,
	DELTA_DIRECTION = 2,
	AREA = 4,
	INFINITE_AREA = 8
};

class Light
{
public:
	Light(LightFlag flag, const Transform &xform, uint32_t sample = 1);
	~Light();

	bool isDeltaLight() const;

	// Evaluate incident radiance arriving at a given point
	virtual Spectrum evalLi(Vector3f &retWi,
							const Intersection &isec,
							const Point2f &u) const = 0;

	// TODO: Use ray differential in the future
	// Evaluate emitted radiance that escapes the scene bounds (no intersection test).
	// Mainly used for environment map, distance light, etc).
	virtual Spectrum evalLe(const Ray &ray) const;

	// Total emitted power
	virtual Spectrum totalEmission() const = 0;

public:
	LightFlag mFlag;
	uint32_t  mSampleCount;

protected:
	Transform mWorldToLight;
};

}
