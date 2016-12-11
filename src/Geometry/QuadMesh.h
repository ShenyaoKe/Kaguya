#pragma once

#include "Math/Geometry.h"
#include "Geometry/PolyMesh.h"

class QuadMesh : public PolyMesh
{
public:
    QuadMesh(vector<Point3f> &inVerts, vector<uint32_t> &vid);
	~QuadMesh();

	void bounding();

	void refine(vector<Shape*> &refined);
	void printInfo(const string &msg = "") const;

	bool intersect(const Ray &inRay,
		           DifferentialGeometry* dg,
		           Float* tHit, Float* rayEpsilon) const;
	void postIntersect(const Ray &inRay, DifferentialGeometry* dg) const;

    void getBufferObject(BufferTrait* vertTraits,
                         BufferTrait* vidTraits) const;
	void exportVBO(vector<float>* vtx_array  = nullptr,
		           vector<float>* uv_array   = nullptr,
		           vector<float>* norm_array = nullptr) const;
	
	void exportIndexedVBO(vector<float>*    vtx_array   = nullptr,
		                  vector<float>*    uv_array    = nullptr,
		                  vector<float>*    norm_array  = nullptr,
		                  vector<uint32_t>* idx_array   = nullptr) const;

    vector<Point3f>  vertices() const { return verts; }
    vector<uint32_t> faceids() const { return vids; }

protected:
    vector<Point3f>  verts;
    vector<uint32_t> vids;
};
