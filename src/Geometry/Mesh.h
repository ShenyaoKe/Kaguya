#pragma once
#ifndef __MESH__
#define __MESH__

#ifndef KAGUYA_DOUBLE_AS_FLOAT
#define KAGUYA_DOUBLE_AS_FLOAT
#endif // !KAGUYA_DOUBLE_AS_FLOAT

//#include "Core/rtdef.h"
#include "Geometry/Shape.h"
#include "Math/Geometry.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <vector>

struct TriangleFaceIndex
{
	int vtx[3];//vertex
	int uv[3];//texture coordinate
	int n[3];//normal
	void printInfo(const string &msg = "") const
	{
		if (!msg.empty())
		{
			cout << msg << endl;
		}
		for (int i = 0; i < 3; i++)
		{
			cout << vtx[i] << "/" << uv[i] << "/" << n[i] << "\t";
		}
		cout << endl;
	}
};

class Mesh :public Shape
{
protected:
	vector<Point3f*> vertices;
	vector<Point2f*> uvs;
	vector<Normal3f*> normals;
	vector<TriangleFaceIndex*> fids;
	//vector<Triangle> faces;
public:
	friend class Triangle;
	//Mesh();
	Mesh(const char* filename);
	~Mesh();

	void bounding();

	void refine(vector<Shape*> &refined);
	bool loadOBJ(const char* filename);
	void printInfo(const string &msg = "") const;

	bool intersect(const Ray& inRay,
		DifferentialGeometry* queryPoint,
		Float *tHit, Float *rayEpsilon) const;
	void getNormal(const DifferentialGeometry* queryPoint) const;
	void getUV(const DifferentialGeometry* queryPoint) const;

	template <typename vbo_t>
	void exportVBO(int &size, vbo_t* &vtx_array,
		vbo_t* &uv_array, vbo_t* &norm_array) const;
	template <typename vbo_t>
	void exportVBO(int &size, vbo_t** vtx_array = nullptr,
		vbo_t** uv_array = nullptr, vbo_t** norm_array = nullptr,
		int** idx_array = nullptr) const;
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
	Triangle(Mesh *inMesh, int n);
	~Triangle();

	void bounding();
	void attachMesh(const Mesh* inMesh);
	void setPoint(Point3f* p0, Point3f* p1, Point3f* p2);
	void setUV(Point2f* uv0, Point2f* uv1, Point2f* uv2);
	void setNormal(Normal3f* n0, Normal3f* v1, Normal3f* v2);

	bool intersect(const Ray& inRay,
		DifferentialGeometry* queryPoint,
		Float *tHit, Float *rayEpsilon) const;
	void getNormal(DifferentialGeometry* queryPoint) const;
	
	/*friend void exportVertices(Triangle* triface, Float* buffer);
	friend void exportTexCoords(Triangle* triface, Float* buffer);
	friend void exportNormals(Triangle* triface, Float* buffer);*/
protected:
	const Mesh* mesh;
	vector<Point3f*> p;
	vector<Point2f*> uv;
	vector<Normal3f*> n;
	friend class Mesh;
};

template void Mesh::exportVBO(int &size, int** vtx_array, int** uv_array, int** norm_array, int** idx_array) const;
template void Mesh::exportVBO(int &size, float** vtx_array, float** uv_array, float** norm_array, int** idx_array) const;

template void Mesh::exportVBO(int &size, float* &vtx_array, float* &uv_array, float* &norm_array) const;
template void Mesh::exportVBO(int &size, double* &vtx_array, double* &uv_array, double* &norm_array) const;

#endif // __MESH__