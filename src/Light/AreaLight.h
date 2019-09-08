#pragma once
#include "Light/Light.h"

namespace Kaguya
{

class AreaLight : public Light
{
public:
	AreaLight(const Transform &w2l,
			  const Spectrum &intensity, uint32_t sample,
			  const std::shared_ptr<Geometry> &shape);
	~AreaLight();


	// Evaluate incident radiance arriving at a given point
	Spectrum evalLi(Vector3f &retWi,
					const Intersection &isec,
					const Point2f &u) const override;

	// Total emitted power
	Spectrum totalEmission() const override;

	// Evaluate emitted radiance in the given outgoing direction
	Spectrum evalEmission(const Intersection &isec, const Vector3f &w) const;

private:
	std::shared_ptr<const Geometry> mShape;
	Spectrum mIntensity;
	Float mSurfaceArea;
};

}
