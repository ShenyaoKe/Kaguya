#pragma once

#include "Geometry/Geometry.h"
#include "Light/Light.h"

namespace Kaguya
{

class RenderPrimitive
{
public:
	RenderPrimitive();
	RenderPrimitive(std::shared_ptr<Geometry> &geom,
					std::shared_ptr<Light> &light);
	~RenderPrimitive();

	const Geometry* getGeometry() const { return mGeometry.get(); }

private:
	std::shared_ptr<Geometry> mGeometry;
	std::shared_ptr<Light> mLight;
};

}
