#include "Light.h"

namespace Kaguya
{

Light::Light(LightFlag flag, const Transform &xform, uint32_t sample)
	: mFlag(flag)
	, mSampleCount(sample)
	, mWorldToLight(xform)
{
}

Light::~Light()
{
}

bool Light::isDeltaLight() const
{
	return mFlag == LightFlag::DELTA_POSITION
		|| mFlag == LightFlag::DELTA_DIRECTION;
}

Spectrum Light::evalLe(const Ray &ray) const
{
	return Spectrum(0.f);
}

}
