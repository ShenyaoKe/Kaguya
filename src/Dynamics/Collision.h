#pragma once
#ifndef __COLLISION__
#define __COLLISION__

#ifndef KAGUYA_DOUBLE_AS_FLOAT
#define KAGUYA_DOUBLE_AS_FLOAT
#endif // !KAGUYA_DOUBLE_AS_FLOAT

#include "Core/Kaguya.h"
#include "Math/CGVector.h"
#include "Accel/BBox.h"
#include "Geometry/Shape.h"
#include "Geometry/Sphere.h"
#include "Geometry/Mesh.h"
#include "Geometry/DifferentialGeometry.h"

class Collision
{
public:
	static bool collideP(const BBox &box0, const BBox &box1);
	static bool collideP(const BBox &box0, const Matrix4D &b0mat, const BBox &box1);
	static bool collideP(const BBox &box0, const Matrix4D &b0mat,
		const BBox &box1, const Matrix4D &b1mat);
	static bool collideP(const geoSphere &sphere, const BBox &box);
	static bool collideP(const geoSphere &sphere, const Triangle &triangle);
	static bool collideP(const geoSphere &sph0, const geoSphere &sph1);

	static bool collide(const Point3D &prePos, const Point3D &curPos,
		const KdTreeAccel* tree, DifferentialGeometry* queryPoint,
		Float* tHit, Float* hitEpsilon);

	static bool collide(const BBox &targetBound,
		const Matrix4D &treeMat, const KdTreeAccel* tree,
		BBox* &collisionBound, vector<Point3D*> &primpts);
	static bool collide(const BBox &targetBound, const Matrix4D &treeMat,
		const KdAccelNode* treeNode, const KdTreeAccel* tree,
		BBox* &collisionBound, vector<Point3D*> &primpts);
protected:
private:
};


#endif // __COLLISION__