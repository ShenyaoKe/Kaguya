#pragma once

#include "Geometry/PolyMesh.h"

namespace Kaguya
{

class QuadMesh : public PolyMesh
{
public:
	QuadMesh(std::vector<Point3f>             &vertexBuffer,
			 std::vector<uint32_t>            &indexBuffer,
			 std::vector<uint32_t>            &faceSizeBuffer,
			 size_t                            totalPrimCount,
			 std::shared_ptr<TextureAttribute> texAttri,
			 std::shared_ptr<NormalAttribute>  normAttri,
			 bool                              isTessellated = true);
	~QuadMesh();

	void bounding() override;

	void printInfo(const std::string &msg) const override;

	bool intersect(const Ray &inRay,
				   DifferentialGeometry* dg,
				   Float* tHit, Float* rayEpsilon) const override;
	void postIntersect(const Ray &inRay,
					   DifferentialGeometry* dg) const override;

	static size_t getFaceSize() { return sQuadFaceSize; }
	void getTessellated(TessBuffer &trait) const override;

	PolyMeshType polyMeshType() const override
	{
		return PolyMeshType::QUAD;
	}

private:
	void tessellate(std::vector<uint32_t> &indexBuffer,
					std::vector<uint32_t> &faceSizeBuffer,
					size_t                 tessellatedCount) override;

	const static uint32_t sQuadFaceSize = 4;

};

}
