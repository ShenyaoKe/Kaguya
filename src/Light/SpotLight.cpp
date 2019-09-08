#include "SpotLight.h"

namespace Kaguya
{

SpotLight::SpotLight(const Transform &xform, const Spectrum &intensity,
					 Float coneAngle, Float falloffAngle)
	: Light(LightFlag::DELTA_POSITION, xform)
	, mPosistion(mWorldToLight.invXform(Point3f(0, 0, 0)))
	, mIntensity(intensity)
	, mCosConeAngle(std::cos(degreeToRadian(coneAngle)))
	, mCosFalloffAngle(std::cos(degreeToRadian(falloffAngle)))
	, mInvFalloffRate(1 / (mCosFalloffAngle - mCosConeAngle))
{
}

SpotLight::~SpotLight()
{
}

Spectrum SpotLight::evalLi(Vector3f &retWi,
						   const Intersection &isec,
						   const Point2f &/*u*/) const
{
	retWi = mPosistion - isec.mPos;
	double distSq = retWi.lengthSquared();
	retWi /= std::sqrt(distSq);

	return mIntensity * falloff(-retWi) / distSq;
}

Spectrum SpotLight::totalEmission() const
{
	return mIntensity * M_TWOPI * (1 - 0.5f * (mCosConeAngle + mCosFalloffAngle));
}

Float SpotLight::falloff(const Vector3f &wi) const
{
	Vector3f unitW = normalize(mWorldToLight(wi));
	Float cosTheta = unitW.z;
	if (cosTheta < mCosConeAngle)
	{
		return 0;
	}
	if (cosTheta > mCosFalloffAngle)
	{
		return 1;
	}

	return quatric((cosTheta - mCosFalloffAngle) * mInvFalloffRate);
}
}
