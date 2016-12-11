#pragma once
#include "Geometry/Mesh.h"

class PolyMesh : public Mesh
{
public:
    PolyMesh() {}
    PolyMesh(vector<Point3f>  &inVerts,
             vector<uint32_t> &inIndices);
    virtual ~PolyMesh() = 0;

protected:
    vector<Point3f>  verts;
    vector<uint32_t> vids;
};

enum class AttributesType
{
    CONSTANT,
    UNIFORM,
    VERTEX_VARYING,
    FACE_VARYING,
};

class PolyAttributes
{
public:
	PolyAttributes() {}
	~PolyAttributes() {}
};


