#include "Geometry/PolyMesh.h"
#include "Tracer/Ray.h"
#include "Geometry/DifferentialGeometry.h"
#include "Shading/Shader.h"
#include "Shading/TextureMapping.h"
#include "Shading/Texture.h"
#include "QuadMesh.h"

QuadMesh::QuadMesh(vector<Point3f>  &inVerts,
                   vector<uint32_t> &inIndices)
    : PolyMesh(inVerts, inIndices)
{
}

QuadMesh::~QuadMesh()
{
}

void QuadMesh::bounding()
{
	for (auto &v : verts)
	{
		ObjBound.Union(v);
	}
}

void QuadMesh::refine(vector<Shape*> &refined)
{
}

void QuadMesh::printInfo(const string &msg) const
{
	if (!msg.empty())
	{
		cout << msg << endl;
	}
	for (int i = 0; i < verts.size(); i++)
	{
		cout << "Vertex:\t" << verts[i] << endl;
	}
	for (int i = 0; i < vids.size(); i++)
	{
		/*cout << "Faces:\t";
		vids[i].printInfo();*/
	}
}

bool QuadMesh::intersect(const Ray &inRay, DifferentialGeometry* dg,
                             Float* tHit, Float* rayEpsilon) const
{
	return false;
}

void QuadMesh::postIntersect(const Ray &inRay, DifferentialGeometry* dg) const
{
	// TODO: Implement post-intersection method
}

void QuadMesh::getBufferObject(BufferTrait* vertTraits,
                                   BufferTrait* vidTraits) const
{
    if (vertTraits)
    {
        vertTraits->data    = (void*)(verts.data());
        vertTraits->count   = verts.size();
        vertTraits->size    = sizeof(Point3f) * vertTraits->count;
        vertTraits->offset  = 0;
        vertTraits->stride  = sizeof(Point3f);
    }
    if (vidTraits)
    {
        vidTraits->data     = (void*)(vids.data());
        vidTraits->count    = vids.size();
        vidTraits->size     = sizeof(uint32_t) * vidTraits->count;
        vidTraits->offset   = 0;
        vidTraits->stride   = sizeof(uint32_t);
    }
}

void QuadMesh::exportVBO(vector<float>* vtx_array,
	                     vector<float>* uv_array,
	                     vector<float>* norm_array) const
{
}

void QuadMesh::exportIndexedVBO(vector<float>* vtx_array,
                                vector<float>* uv_array,
	                            vector<float>* norm_array,
                                vector<uint32_t>* idx_array) const
{
	bool has_vert(false), has_texcoord(false), has_normal(false), has_uid(false);
	
	if (vtx_array != nullptr)
	{
		vtx_array->clear();
		vtx_array->reserve(verts.size() * 3);
		has_vert = true;
	}
}
