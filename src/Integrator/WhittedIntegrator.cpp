//
// Created by Shenyao Ke on 2019-02-18.
//

#include "WhittedIntegrator.h"
#include "Core/Scene.h"
#include "Geometry/Intersection.h"

namespace Kaguya
{


WhittedIntegrator::WhittedIntegrator(uint32_t maxDepth,
                                     const Bounds2i &pixelRange,
                                     Sampler &sampler)
    : SampleIntegrator(pixelRange, sampler)
    , mMaxDepth(maxDepth)
{
}

Spectrum WhittedIntegrator::evalLi(Ray &ray, const Scene &scene, const Sampler &sampler, uint32_t rayDepth)
{
	Intersection isect;

	if (!scene.intersect(ray, &isect))
	{
		// If no intersection, get radiance directly from light
		Spectrum lightSpec(0.f);
		for (auto &light : scene.getLights())
		{
			lightSpec += light->evalLe(ray);
		}

		return lightSpec;
	}

	for (auto &light : scene.getLights())
	{
		// Test ray isect->light visibility

		// accumulate radiance if visible
		Spectrum lightSpec = light->evalLi(ray.d, isect, sampler.generate2D());
	}

	if (rayDepth < mMaxDepth)
	{
		// If Specular Reflect
		//     Trace reflection
		// If specular transmit
		//     Trace transmission
	}
	return Kaguya::Spectrum();
}

}