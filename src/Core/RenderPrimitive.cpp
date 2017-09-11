#include "RenderPrimitive.h"

namespace Kaguya
{

RenderPrimitive::RenderPrimitive()
{
}

RenderPrimitive::RenderPrimitive(std::shared_ptr<Geometry> &geom,
								 std::shared_ptr<Light> &light)
	: mGeometry(geom), mLight(light)
{
}

RenderPrimitive::~RenderPrimitive()
{
}

}
