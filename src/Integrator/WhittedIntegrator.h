#pragma once

#include "Integrator.h"

namespace Kaguya
{

class WhittedIntegrator : public SampleIntegrator
{
public:
	WhittedIntegrator(uint32_t maxDepth,
	                  const Bounds2i &pixelRange,
	                  Sampler &sampler);

	Spectrum evalLi(Ray &ray, const Scene &scene, const Sampler &sampler, uint32_t rayDepth) override;
private:
	uint32_t mMaxDepth;
};

}