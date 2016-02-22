#include "Math/Transform.h"

Vector4D Transform::operator()(const Vector4D &vec) const
{
	Vector4D ret = m * vec;
	if (ret.w != 0.0 || ret.w != 1.0)
	{
		ret /= ret.w;
	}
	return ret;
}
Vector3D Transform::operator()(const Vector3D &vec, Float w) const
{
	return m(vec, w);
	/*Vector4D ret = m * Vector4D(vec, w);
	if (ret.w != 0.0 || ret.w != 1.0)
	{
		ret /= ret.w;
	}
	return Vector3D(ret.x, ret.y, ret.z);*/
}

BBox Transform::operator()(const BBox &b) const
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

void Transform::operator()(const Ray& ray, Ray* ret) const
{
	ret->d = m(ray.d, 0.0);
	ret->o = m(ray.o, 1.0);
	if (&ray != ret)
	{
		ret->tmin = ray.tmin;
		ret->tmax = ray.tmax;
		ret->time = ray.time;
		ret->dp = ray.dp;
	}
}

Vector4D Transform::xformNormal(const Vector3D &vec) const
{
	return Vector4D();
}

Matrix4D Transform::getMat() const
{
	return m;
}

Matrix4D Transform::getInvMat() const
{
	return mInv;
}

void Transform::setMat(const Matrix4D &mat)
{
	m = mat;
	mInv = m.Inverse();
}

void Transform::setInvMat(const Matrix4D &matInv)
{
	mInv = matInv;
	m = mInv.Inverse();
}

Transform xformTRS(
	Float tx, Float ty, Float tz,
	Float rx, Float ry, Float rz,
	Float sx, Float sy, Float sz
	);
//Transform Rotate(const Vector3D &axis, Float theta);

Transform xformTRS(
	Float tx, Float ty, Float tz,
	Float rx, Float ry, Float rz,
	Float sx, Float sy, Float sz)
{
	Matrix4D T = Matrix4D::Translate(tx, ty, tz);
	Matrix4D R = Matrix4D::Rotate(rx, ry, rz);
	Matrix4D S = Matrix4D::Scale(sx, sy, sz);
	return Transform(T * R * S);
}

Transform Rotate(const Vector3D &axis, Float theta)
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

Transform lookAt(const Point3D &pos = Point3D(0, 0, 0),
	const Point3D &target = Point3D(0, 0, 1),
	const Vector3D &up = Point3D(0, 1, 0))
{
	//Camera to World
	Vector3D nz = target - pos;//pos - target;
	// distance between target and camera position is too small
	if (isFuzzyNull(nz.x) && isFuzzyNull(nz.y) && isFuzzyNull(nz.z))
	{
		return Transform();
	}
	
	nz.normalize();
	//Vector3D nx = Normalize(Cross(up, nz));//left dir
	//Vector3D ny = Cross(nz, nx);
#ifdef RIGHT_HAND_ORDER // OpenGL style
	Vector3D nx = Normalize(Cross(up, nz));
	Vector3D ny = Cross(nz, nx);
	Float mat[4][4] = {
		nx.x, nx.y, nx.z, 0.0,
		ny.x, ny.y, ny.z, 0.0,
		-nz.x, -nz.y, -nz.z, 0.0,
		pos.x, pos.y, pos.z, 1.0
	};
#else // DirectX style
	nz = -nz;
	Vector3D nx = Normalize(Cross(nz, up));//left dir
	Vector3D ny = Cross(nx, nz);
	Float mat[4][4] = {
		nx.x, nx.y, nx.z, 0.0,
		ny.x, ny.y, ny.z, 0.0,
		nz.x, nz.y, nz.z, 0.0,
		pos.x, pos.y, pos.z, 1.0
	};
#endif

	return Transform(mat);
}