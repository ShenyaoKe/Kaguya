#pragma once

#include "Core/Kaguya.h"
#include "Core/Sampler.h"
#include "Accel/Bounds.h"
#include "Tracer/Ray.h"
#include "Light/Spectrum.h"

namespace Kaguya
{

class Scene;

class Integrator
{
public:
    virtual ~Integrator() {}
    virtual void render(const Scene &scene) = 0;
};

class SampleIntegrator : public Integrator
{
public:
	explicit SampleIntegrator(const Bounds2i &pixelRange,
                              Sampler &sampler)
		: mPixelRange(pixelRange)
		, mSampler(sampler)
	{}
	void render(const Scene &scene) override;
	virtual void preprocess(const Scene &/*scene*/, Sampler &/*sampler*/) {}
	virtual Spectrum evalLi(Ray &ray, const Scene &scene, const Sampler &sampler, uint32_t rayDepth = 0) = 0;

protected:
	Bounds2i mPixelRange;
	// Sampler is used to sample on image plane or light.
	Sampler& mSampler;
};

}
