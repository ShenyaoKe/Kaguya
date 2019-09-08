#pragma once

#include "Integrator/Integrator.h"

namespace Kaguya
{

class PathIntegrator : public SampleIntegrator
{
public:
	PathIntegrator(const Bounds2i &pixelRange,
                   Sampler &sampler)
		: SampleIntegrator(pixelRange, sampler)
	{}

	void render(const Scene &scene) override;

};

}

