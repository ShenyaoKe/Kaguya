#pragma once
#include "Geometry/PolyMesh.h"

namespace Kaguya
{

class TriangleMesh : public PolyMesh
{
public:
	TriangleMesh(std::vector<Point3f>              vertexBuffer,
				 std::vector<uint32_t>             indexBuffer,
				 const std::vector<uint32_t>      &faceSizeBuffer,
				 size_t                            totalPrimCount,
				 std::shared_ptr<TextureAttribute> texAttri,
				 std::shared_ptr<NormalAttribute>  normAttri,
				 bool                              isTessellated = true);
	~TriangleMesh();

	void bounding() override;

	void printInfo(const std::string &msg) const override;

	bool intersect(const Ray &inRay,
				   Intersection* isec,
				   Float* tHit, Float* rayEpsilon) const override;
	void postIntersect(const Ray &inRay,
					   Intersection* isec) const override;

	static size_t getFaceSize() { return sTriFaceSize; }
	void getTessellated(TessBuffer &trait) const override;

	PolyMeshType polyMeshType() const override
	{
		return PolyMeshType::TRIANGLE;
	}

private:
	void tessellate(std::vector<uint32_t>       &indexBuffer,
					const std::vector<uint32_t> &faceSizeBuffer,
					size_t                       tessellatedCount) override;

	const static size_t sTriFaceSize = 3;
};

namespace TriangleUtils
{
bool intersect(const Point3f &p0,
			   const Point3f &p1,
			   const Point3f &p2,
			   Ray &inRay,
			   Float* tHit,
			   Float* rayEpsilon);

bool intersectWatertight(const Point3f &p0,
						 const Point3f &p1,
						 const Point3f &p2,
						 Ray &inRay, Float* tHit, Float* rayEpsilon);

void postIntersect(const Point3f &p0,
				   const Point3f &p1,
				   const Point3f &p2,
				   const Ray &inRay, Intersection* isec);

}

}
