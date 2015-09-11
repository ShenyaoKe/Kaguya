#pragma once
#ifndef __TRANSFORM__
#define __TRANSFORM__
#include "Math/CGVector.h"
#include "Math/Matrix4D.h"

class Transform
{
public:

	Transform()
	{
		m.setIdentity();
		mInv.setIdentity();
	}
	Transform(const double mat[4][4])
		: m(mat[4][4])
	{
		mInv = m.inverseMat();
	}
	Transform(const Matrix4D &mat)
		: m(mat), mInv(mat.inverseMat())
	{}
	Transform(const Matrix4D &mat, const Matrix4D &matInv)
		: m(mat), mInv(matInv)
	{}
	~Transform() {}

	
	Vector4D operator () (const Vector4D &vec) const;
	Vector4D xformNormal(const Vector3D &vec) const;

	friend Transform inverse(const Transform &t)
	{
		return Transform(t.mInv, t.m);
	}
	friend Transform transpose(const Transform &t)
	{
		return Transform(t.m.transposeMat(),
			t.mInv.transposeMat());
	}
private:
	Matrix4D m, mInv;
	friend class Quaternion;
};
Vector4D Transform::operator()(const Vector4D &vec) const
{
	Vector4D ret = m * vec;
	if (ret.w != 0.0 || ret.w != 1.0)
	{
		ret /= ret.w;
	}
	return ret;
}

Vector4D Transform::xformNormal(const Vector3D &vec) const
{

}
Transform xformTRS(
	double tx, double ty, double tz,
	double rx, double ry, double rz,
	double sx, double sy, double sz
	);
Transform Rotate(const Vector3D &axis, double theta);
Transform lookAt(const Point3D &pos, const Point3D &target, const Vector3D &up);

Transform xformTRS(
	double tx, double ty, double tz,
	double rx, double ry, double rz,
	double sx, double sy, double sz)
{
	Matrix4D T = setTranslation(tx, ty, tz);
	Matrix4D R = setRotation(rx, ry, rz);
	Matrix4D S = setScale(sx, sy, sz);
	return Transform(T * R * S);
}

Transform Rotate(const Vector3D &axis, double theta)
{
	Vector3D u = Normalize(axis);
	double rad = DegreeToRadian(theta);
	double c = cos(rad);
	double s = sin(rad);
	double t = 1 - c;

	double mat[4][4];

	mat[0][0] = t * sqr(u.x) + c;
	mat[0][1] = t * u.x * u.y - s * u.x;
	mat[0][2] = t * u.x * u.z + s * u.y;
	mat[0][3] = 0.0;

	mat[1][0] = t * u.x * u.y + s * u.z;
	mat[1][1] = t * sqr(u.y) + c;
	mat[1][2] = t * u.y * u.z - s * u.x;
	mat[1][3] = 0.0;

	mat[2][0] = t * u.x * u.z - s * u.y;
	mat[2][1] = t * u.y * u.z + s * u.x;
	mat[2][2] = t * sqr(u.z) + c;
	mat[2][3] = 0.0;

	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;
	mat[3][3] = 1.0;

	return Transform(mat);
}

Transform lookAt(const Point3D &pos, const Point3D &target, const Vector3D &up)
{
	//Camera to World
	Vector3D nz = Normalize(target - pos);
	Vector3D nx = Normalize(Cross(nz, up));//right dir
	Vector3D ny = Cross(nz, nx);

	double mat[4][4] = {
		nx.x, ny.x, nz.x, pos.x,
		nx.y, ny.y, nz.y, pos.y,
		nx.z, ny.z, nz.z, pos.z,
		0.0, 0.0, 0.0, 1.0
	};

	return Transform(mat);
}

#endif // __TRANSFORM__