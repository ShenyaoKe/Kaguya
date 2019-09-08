#include "Integrator.h"
#include "Core/Scene.h"
#include "Camera/Camera.h"

namespace Kaguya
{

void SampleIntegrator::render(const Scene &scene)
{
	auto& camera = scene.getCamera();
	uint32_t width = camera->getFilm().width;
	uint32_t height = camera->getFilm().height;

	Ray ray;
	uint32_t sampleCount = 2;
	std::vector<Spectrum> img(width * height);
	for (uint32_t i = 0; i < width; ++i)
	{
		for (uint32_t j = 0; j < height; ++j)
		{
			for (uint32_t k = 0; k < sampleCount * sampleCount; ++k)
			{
				CameraSample sample{ mSampler.generate2D(),
									 mSampler.generate2D(),
									 mSampler.generate1D() };
				camera->generateRay(sample, &ray);
				Spectrum L(0.f);
				L = evalLi(ray, scene, mSampler, 0);
				img[i + j * height] += L;
			}
		}
	}
}

}