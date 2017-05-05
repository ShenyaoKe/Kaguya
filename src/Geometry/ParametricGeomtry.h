#pragma once
#include "Geometry/Primitive.h"

namespace Kaguya
{

class ParametricGeomtry : public Primitive
{
public:
	ParametricGeomtry(const Transform* o2w = nullptr)
		: Primitive(o2w)
	{
	}
	~ParametricGeomtry() {}

	PrimitiveType primitiveType() const override
	{
		return PrimitiveType::PARAMATRIC_SURFACE;
	}

	void getRenderBuffer(RenderBufferTrait* trait) const override;

protected:
	std::shared_ptr<PolyMesh> mProxyMesh;
};

}
