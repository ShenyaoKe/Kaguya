#include "ParametricGeomtry.h"
#include "Geometry/PolyMesh.h"

namespace Kaguya
{

void ParametricGeomtry::getRenderBuffer(RenderBufferTrait* trait) const
{
	mProxyMesh->getRenderBuffer(trait);
}

}
