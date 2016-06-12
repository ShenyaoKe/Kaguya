#pragma once
#ifndef __MESH__
#define __MESH__

#include "Geometry/Shape.h"
#include "Math/Geometry.h"

struct PolyIndex
{
	PolyIndex(size_t sz = 3) : size(sz)
	{
		v.reserve(sz);
		uv.reserve(sz);
		n.reserve(sz);
	}
	void push_back(uint32_t* ids)
	{
		v.push_back(ids[0]);
		if (ids[1] > 0) uv.push_back(ids[1]);
		if (ids[2] > 0) n.push_back(ids[2]);
	}
	void printInfo(const string &msg = "") const
	{
		if (!msg.empty())
		{
			cout << msg << endl;
		}
		for (int i = 0; i < size; i++)
		{
			cout << v[i] << "/" << uv[i] << "/" << n[i] << "\t";
		}
		cout << endl;
	}

	size_t size;
	ui32s_t v, uv, n;
};

namespace ObjParser
{
	bool parse(const char* filename,
		vector<Point3f> &verts,
		vector<Point2f> &uvs, vector<Normal3f> &norms,
		vector<PolyIndex> &polys);

	enum index_t : uint8_t
	{
		V = 1 << 0,
		UV = 1 << 1,
		NORM = 1 << 2,
		VT = V | UV,
		VN = V | NORM,
		VTN = V | UV | NORM
	};

	inline index_t facetype(const char* str, uint32_t* val)
	{
		int argv = sscanf(str, "%d/%d/%d", val, val + 1, val + 2);
		switch (argv)
		{
		case 3:// V/T/N
			return VTN;//111
		case 2:// V/T
			return VT;//011
		case 1:
			argv = sscanf(str, "%d//%d", val, val + 2);
			if (argv == 2)// V//N
			{
				return VN;//101
			}
			else// V
			{
				return V;//001
			}
		default:
			return V;
		}
	}
};

class TriangleMesh :public Shape
{
protected:
	vector<Point3f> verts;
	vector<Point2f> uvs;
	vector<Normal3f> norms;
	vector<PolyIndex> fids;
	//vector<Triangle> faces;
public:
	friend class Triangle;
	//Mesh();
	TriangleMesh(const char* filename);
	~TriangleMesh();

	void bounding();

	void refine(vector<Shape*> &refined);
	bool loadOBJ(const char* filename);
	void printInfo(const string &msg = "") const;

	bool intersect(const Ray& inRay,
		DifferentialGeometry* dg,
		Float *tHit, Float *rayEpsilon) const;
	void postIntersect(const Ray& inRay, DifferentialGeometry* dg) const;

	void exportVBO(
		vector<float>* vtx_array = nullptr,
		vector<float>* uv_array = nullptr,
		vector<float>* norm_array = nullptr) const;
	
	void exportIndexedVBO(
		vector<float>* vtx_array = nullptr,
		vector<float>* uv_array = nullptr,
		vector<float>* norm_array = nullptr,
		vector<unsigned int>* idx_array = nullptr) const;
};
class Triangle : public Shape
{
public:
	Triangle();
	Triangle(TriangleMesh* inMesh, size_t n);

	void bounding();
	void attachMesh(const TriangleMesh* inMesh);
	void setPoint(Point3f* p0, Point3f* p1, Point3f* p2);
	void setUV(Point2f* uv0, Point2f* uv1, Point2f* uv2);
	void setNormal(Normal3f* n0, Normal3f* v1, Normal3f* v2);

	bool intersect(const Ray& inRay,
		DifferentialGeometry* dg,
		Float *tHit, Float *rayEpsilon) const;
	void postIntersect(const Ray& inRay,
		DifferentialGeometry* dg) const;
	void getNormal(DifferentialGeometry* queryPoint) const;
	
	/*friend void exportVertices(Triangle* triface, Float* buffer);
	friend void exportTexCoords(Triangle* triface, Float* buffer);
	friend void exportNormals(Triangle* triface, Float* buffer);*/
protected:
	const TriangleMesh* mesh;
	array<Point3f*, 3> p;
	array<Point2f*, 3> uv;
	array<Normal3f*, 3> n;
	friend class TriangleMesh;
};

#endif // __MESH__