#pragma once
#include "Geometry/Mesh.h"

namespace Kaguya
{

class SubdMesh : public Mesh
{
public:
	SubdMesh();
	~SubdMesh();

	PrimitiveType primitiveType() const override
	{
		return PrimitiveType::SUBDIVISION_MESH;
	}
};

}
