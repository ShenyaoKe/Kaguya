#include "PointLight.h"

namespace Kaguya
{

PointLight::PointLight(const Transform &xform, const Spectrum &intensity)
	: Light(LightFlag::DELTA_POSITION, xform)
	, mPosition(mWorldToLight.invXform(Point3f(0, 0, 0)))
	, mIntensity(intensity)
{
}

PointLight::~PointLight()
{
}

Spectrum PointLight::evalLi(Vector3f &retWi,
							const Intersection &isec,
							const Point2f &) const
{
	retWi = mPosition - isec.mPos;
	Float dist = retWi.lengthSquared();
	retWi /= std::sqrt(dist);

	return mIntensity / dist;
}

Spectrum PointLight::totalEmission() const
{
	// SurfaceArea (4 * Pi * R^2) * intensity
	return 4 * M_PI * mIntensity;
}
}
