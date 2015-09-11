#pragma once
#ifndef __MESH__
#define __MESH__

#include "Core/rtdef.h"
#include "Geometry/Shape.h"
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
	void printInfo() const
	{
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
	vector<Point3D*> vertices;
	vector<Point2D*> uvs;
	vector<Vector3D*> normals;
	vector<TriangleFaceIndex*> fids;
	//vector<Triangle> faces;
public:
	friend class Triangle;
	Mesh();
	Mesh(const char* filename);
	~Mesh();

	void bounding();

	void refine(vector<Shape*> &refined);
	bool loadOBJ(const char* filename);
	void printInfo() const;

	bool getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, double *tHit, double *rayEpsilon) const;
	void getNormal(const DifferentialGeometry *queryPoint) const;
	void getUV(const DifferentialGeometry *queryPoint) const;

	friend void exportVBO(Mesh* tri_mesh, int &size, double* &verts, double* &texcoord, double* &nms);
};
class Triangle :public Shape
{
public:
	Triangle();
	/*Triangle(Mesh* &inMesh, Point3D& p0, Point3D& p1, Point3D& p2,
		Point2D& uv0, Point2D& uv1, Point2D& uv2,
		Vector3D& n0, Vector3D& n1, Vector3D& n2);*/
	Triangle(Mesh *inMesh, int n);
	~Triangle();

	void bounding();
	void attachMesh(const Mesh* inMesh);
	void setPoint(Point3D* p0, Point3D* p1, Point3D* p2);
	void setUV(Point2D* uv0, Point2D* uv1, Point2D* uv2);
	void setNormal(Vector3D* n0, Vector3D* v1, Vector3D* v2);

	bool getDifferentialGeometry(const Ray& inRay, DifferentialGeometry *queryPoint, double *tHit, double *rayEpsilon) const;
	void getNormal(const DifferentialGeometry *queryPoint) const;
	void getUV(const DifferentialGeometry *queryPoint) const;
	ColorRGBA getColor(const DifferentialGeometry *queryPoint, const Light* light) const;
	
	/*friend void exportVertices(Triangle* triface, double* buffer);
	friend void exportTexCoords(Triangle* triface, double* buffer);
	friend void exportNormals(Triangle* triface, double* buffer);*/
protected:
	const Mesh* mesh;
	vector<Point3D*> p;
	vector<Point2D*> uv;
	vector<Point3D*> n;
private:
};
#endif // __MESH__