#pragma once
#include "Geometry/Geometry.h"

namespace Kaguya
{

class ParametricGeomtry : public Geometry
{
public:
	ParametricGeomtry(const Transform* o2w = nullptr)
		: Geometry(o2w)
	{
	}
	~ParametricGeomtry() {}

	GeometryType primitiveType() const override
	{
		return GeometryType::PARAMATRIC_SURFACE;
	}

	void getRenderBuffer(RenderBufferTrait* trait) const override;

protected:
	std::shared_ptr<PolyMesh> mProxyMesh;
};

}
