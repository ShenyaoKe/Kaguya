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
//#include "Math/CGVector.h"
#include "Math/Matrix4D.h"
#include "Accel/BBox.h"

class Transform
{
public:

	Transform()
	{
		m.setIdentity();
		mInv.setIdentity();
	}
	Transform(const Float mat[4][4])
	{
		m = Matrix4D(
			mat[0][0], mat[0][1], mat[0][2], mat[0][3],
			mat[1][0], mat[1][1], mat[1][2], mat[1][3],
			mat[2][0], mat[2][1], mat[2][2], mat[2][3],
			mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
		mInv = m.Inverse();
	}
	Transform(const Matrix4D &mat)
		: m(mat), mInv(mat.Inverse())
	{}
	Transform(const Matrix4D &mat, const Matrix4D &matInv)
		: m(mat), mInv(matInv)
	{}
	~Transform() {}

	Vector3D operator () (const Vector3D &vec, Float w) const;
	Vector4D operator () (const Vector4D &vec) const;
	Point3f operator () (const Point3f &pos) const;
	Vector3f operator () (const Vector3f &vec) const;
	BBox operator () (const BBox &bbox) const;
	//Ray operator () (const Ray &ray) const;
	void operator () (const Ray& ray, Ray* ret) const;

	//Transform operator * (const Matrix4D &mat);
	Vector4D xformNormal(const Vector3D &vec) const;

	Matrix4D getMat() const;
	Matrix4D getInvMat() const;
	void setMat(const Matrix4D &mat);
	void setInvMat(const Matrix4D &mat);

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
	Matrix4D m, mInv;
	friend class Quaternion;
	friend class Camera;
};
#endif // __TRANSFORM__