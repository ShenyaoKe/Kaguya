#pragma once
#ifndef __TRANSFORM__
#define __TRANSFORM__

#include "Core/Kaguya.h"
#include "Accel/Bounds.h"
#include "Math/Matrix4x4.h"

class Transform
{
public:

	Transform()
	{
	}
	Transform(const Float mat[4][4]) : m(mat), mInv(m.inverse()) {}
	Transform(const Matrix4x4 &mat) : m(mat), mInv(mat.inverse()) {}
	Transform(const Matrix4x4 &mat, const Matrix4x4 &matInv)
		: m(mat), mInv(matInv)
	{}
	~Transform() {}

	Point3f operator () (const Point3f &p) const;
	Vector3f operator () (const Vector3f &v) const;
	Normal3f operator () (const Normal3f &n) const;
	Bounds3f operator () (const Bounds3f &bbox) const;
	Ray operator () (const Ray &ray) const;
	void operator () (const Ray &ray, Ray* ret) const;

	//Transform operator * (const Matrix4x4 &mat);

	const Matrix4x4 &getMat() const { return m; }
	const Matrix4x4 &getInvMat() const { return mInv; }
	void setMat(const Matrix4x4 &mat);
	void setInvMat(const Matrix4x4 &mat);

	friend Transform inverse(const Transform &t)
	{
		return Transform(t.mInv, t.m);
	}
	friend Transform transpose(const Transform &t)
	{
		return Transform(t.m.transpose(), t.mInv.transpose());
	}
private:
	Matrix4x4 m, mInv;
	friend class Quaternion;
	friend class ProjectiveCamera;
};

// b = A * x
// where b is Vector2, A is Matrix2x2,
// x is Vector2
bool solveLinearSystem2x2(
	const Float A[2][2], const Float b[2], Float* x0, Float* x1);
#endif // __TRANSFORM__