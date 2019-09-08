#include "AreaLight.h"

#include "Geometry/Geometry.h"

namespace Kaguya
{

AreaLight::AreaLight(const Transform &w2l,
					 const Spectrum &intensity, uint32_t sample,
					 const std::shared_ptr<Geometry> &shape)
	: Light(LightFlag::AREA, w2l, sample)
	, mShape(shape)
	, mIntensity(intensity)
	, mSurfaceArea(shape->area())
{
}

AreaLight::~AreaLight()
{
}

Spectrum AreaLight::evalLi(Vector3f &/*retWi*/,
						   const Intersection &/*isec*/,
						   const Point2f &/*u*/) const
{
	return Spectrum();
}

Spectrum AreaLight::totalEmission() const
{
	return mIntensity * mSurfaceArea * M_PI;
}

Spectrum AreaLight::evalEmission(const Intersection &isec, const Vector3f &w) const
{
	return dot(isec.mGeomN, w) > 0 ? mIntensity : Spectrum(0.0);
}

}
