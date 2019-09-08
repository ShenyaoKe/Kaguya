#pragma once
#include "Light.h"

namespace Kaguya
{

class PointLight : public Light
{
public:
	PointLight(const Transform &xform, const Spectrum &intensity);
	~PointLight();

	Spectrum evalLi(Vector3f &retWi,
					const Intersection &isec,
					const Point2f &u) const override;

	Spectrum totalEmission() const override;

private:
	Point3f  mPosition;
	Spectrum mIntensity;
};

}
