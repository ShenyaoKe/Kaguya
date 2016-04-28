#pragma once
#ifndef __TRANSFORM__
#define __TRANSFORM__

#ifndef KAGUYA_DOUBLE_AS_FLOAT
#define KAGUYA_DOUBLE_AS_FLOAT
#endif // !KAGUYA_DOUBLE_AS_FLOAT

#ifndef RIGHT_HAND_ORDER
#define RIGHT_HAND_ORDER
#endif

#include "Core/Kaguya.h"
#include "Math/Matrix4x4.h"
#include "Accel/BBox.h"

class Transform
{
public:

	Transform()
	{
	}
	Transform(const Float mat[4][4])
	{
		m = Matrix4x4(
			mat[0][0], mat[0][1], mat[0][2], mat[0][3],
			mat[1][0], mat[1][1], mat[1][2], mat[1][3],
			mat[2][0], mat[2][1], mat[2][2], mat[2][3],
			mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
		mInv = m.Inverse();
	}
	Transform(const Matrix4x4 &mat)
		: m(mat), mInv(mat.Inverse())
	{}
	Transform(const Matrix4x4 &mat, const Matrix4x4 &matInv)
		: m(mat), mInv(matInv)
	{}
	//~Transform() {}

	Point3f operator () (const Point3f &pos) const;
	Vector3f operator () (const Vector3f &vec) const;
	BBox operator () (const BBox &bbox) const;
	Ray operator () (const Ray &ray) const;
	void operator () (const Ray &ray, Ray* ret) const;

	//Transform operator * (const Matrix4x4 &mat);

	Matrix4x4 getMat() const;
	Matrix4x4 getInvMat() const;
	void setMat(const Matrix4x4 &mat);
	void setInvMat(const Matrix4x4 &mat);

	friend Transform inverse(const Transform &t)
	{
		return Transform(t.mInv, t.m);
	}
	friend Transform transpose(const Transform &t)
	{
		return Transform(t.m.Transpose(),
			t.mInv.Transpose());
	}
private:
	Matrix4x4 m, mInv;
	friend class Quaternion;
	friend class Camera;
};
#endif // __TRANSFORM__