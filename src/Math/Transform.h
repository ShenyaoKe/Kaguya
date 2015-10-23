#pragma once
#ifndef __TRANSFORM__
#define __TRANSFORM__

#ifndef KAGUYA_DOUBLE_AS_FLOAT
#define KAGUYA_DOUBLE_AS_FLOAT
#endif // !KAGUYA_DOUBLE_AS_FLOAT


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
		mInv = m.inverseMat();
	}
	Transform(const Matrix4D &mat)
		: m(mat), mInv(mat.inverseMat())
	{}
	Transform(const Matrix4D &mat, const Matrix4D &matInv)
		: m(mat), mInv(matInv)
	{}
	~Transform() {}

	Vector3D operator () (const Vector3D &vec, Float w) const;
	Vector4D operator () (const Vector4D &vec) const;
	BBox operator () (const BBox &bbox) const;
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
		return Transform(t.m.transposeMat(),
			t.mInv.transposeMat());
	}
private:
	Matrix4D m, mInv;
	friend class Quaternion;
	friend class baseCamera;
};
inline Vector4D Transform::operator()(const Vector4D &vec) const
{
	Vector4D ret = m * vec;
	if (ret.w != 0.0 || ret.w != 1.0)
	{
		ret /= ret.w;
	}
	return ret;
}
inline Vector3D Transform::operator()(const Vector3D &vec, Float w) const
{
	Vector4D ret = m * Vector4D(vec, w);
	if (ret.w != 0.0 || ret.w != 1.0)
	{
		ret /= ret.w;
	}
	return Vector3D(ret.x, ret.y, ret.z);
}


inline BBox Transform::operator()(const BBox &b) const
{
	const Transform &M = *this;
	BBox ret(M(b.pMin, 1.0));
	ret = Union(ret, M(Vector3D(b.pMax.x, b.pMin.y, b.pMin.z), 1.0));
	ret = Union(ret, M(Vector3D(b.pMin.x, b.pMax.y, b.pMin.z), 1.0));
	ret = Union(ret, M(Vector3D(b.pMin.x, b.pMin.y, b.pMax.z), 1.0));
	ret = Union(ret, M(Vector3D(b.pMin.x, b.pMax.y, b.pMax.z), 1.0));
	ret = Union(ret, M(Vector3D(b.pMax.x, b.pMax.y, b.pMin.z), 1.0));
	ret = Union(ret, M(Vector3D(b.pMax.x, b.pMin.y, b.pMax.z), 1.0));
	ret = Union(ret, M(Vector3D(b.pMax.x, b.pMax.y, b.pMax.z), 1.0));
	return ret;
}

inline Vector4D Transform::xformNormal(const Vector3D &vec) const
{
	return Vector4D();
}

inline Matrix4D Transform::getMat() const
{
	return m;
}

inline Matrix4D Transform::getInvMat() const
{
	return mInv;
}

inline void Transform::setMat(const Matrix4D &mat)
{
	m = mat;
	mInv = m.inverseMat();
}

inline void Transform::setInvMat(const Matrix4D &matInv)
{
	mInv = matInv;
	m = mInv.inverseMat();
}

inline Transform xformTRS(
	Float tx, Float ty, Float tz,
	Float rx, Float ry, Float rz,
	Float sx, Float sy, Float sz
	);
//inline Transform Rotate(const Vector3D &axis, Float theta);

inline Transform xformTRS(
	Float tx, Float ty, Float tz,
	Float rx, Float ry, Float rz,
	Float sx, Float sy, Float sz)
{
	Matrix4D T = setTranslation(tx, ty, tz);
	Matrix4D R = setRotation(rx, ry, rz);
	Matrix4D S = setScale(sx, sy, sz);
	return Transform(T * R * S);
}

inline Transform Rotate(const Vector3D &axis, Float theta)
{
	Vector3D u = Normalize(axis);
	Float rad = DegreeToRadian(theta);
	Float c = cos(rad);
	Float s = sin(rad);
	Float t = 1 - c;

	Float mat[4][4];

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

inline Transform lookAt(const Point3D &pos = Point3D(0, 0, 0),
	const Point3D &target = Point3D(0, 0, 1),
	const Vector3D &up = Point3D(0, 1, 0))
{
	//Camera to World
	Vector3D nz = Normalize(target - pos);
	Vector3D nx = Normalize(Cross(up, nz));//left dir
	Vector3D ny = Cross(nz, nx);
	
	/*Float mat[4][4] = {
		nx.x, ny.x, nz.x, pos.x,
		nx.y, ny.y, nz.y, pos.y,
		nx.z, ny.z, nz.z, pos.z,
		0.0, 0.0, 0.0, 1.0
	};*/
	Float mat[4][4] = {
		nx.x, nx.y, nx.z, 0.0,
		ny.x, ny.y, ny.z, 0.0,
		nz.x, nz.y, nz.z, 0.0,
		pos.x, pos.y, pos.z, 1.0
	};

	return (Transform(mat));
}

#endif // __TRANSFORM__