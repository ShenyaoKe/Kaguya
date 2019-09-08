#pragma once
#include "Light.h"

namespace Kaguya
{

class SpotLight : public Light
{
public:
	SpotLight(const Transform &xform, const Spectrum &intensity,
			  Float coneAngle, Float falloffAngle);
	~SpotLight();

	// Evaluate incident radiance arriving at a given point
	Spectrum evalLi(Vector3f &retWi,
					const Intersection &isec,
					const Point2f &u) const override;

	// Total emitted power
	Spectrum totalEmission() const override;

private:
	Float falloff(const Vector3f &wi) const;

private:
	Point3f mPosistion;
	Spectrum mIntensity;

	Float mCosConeAngle;
	Float mCosFalloffAngle;

	// 1.0 / (cos(FalloffAngle) - cos(ConeAngle))
	Float mInvFalloffRate;
};

}
