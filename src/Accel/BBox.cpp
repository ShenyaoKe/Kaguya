#include "Accel/BBox.h"

BBox::BBox() : pMin(INFINITY, INFINITY, INFINITY), pMax(-INFINITY, -INFINITY, -INFINITY)
{
}


BBox::BBox(const Point3f &p) :pMin(p), pMax(p)
{
}
BBox::BBox(const Point3f &p1, const Point3f &p2)
{
	pMin = Point3f(min(p1.x, p2.x), min(p1.y, p2.y), min(p1.z, p2.z));
	pMax = Point3f(max(p1.x, p2.x), max(p1.y, p2.y), max(p1.z, p2.z));
}

BBox::BBox(const vector<Point3f*> &pts) : pMin(INFINITY, INFINITY, INFINITY), pMax(-INFINITY, -INFINITY, -INFINITY)
{
	for (int i = 0; i < pts.size(); i++)
	{
		this->Union(*pts[i]);
	}
}

BBox::BBox(const BBox &bound, const Matrix4x4 &mat)
{
	for (int i = 0; i < 3; i++)
	{
		pMin[i] = pMax[i] = mat[3][i];
		for (int j = 0; j < 3; j++)
		{
			double e = mat[j][i] * bound.pMin[j];
			double f = mat[j][i] * bound.pMax[j];
			if (e < f)
			{
				pMin[i] += e;
				pMax[i] += f;
			} 
			else
			{
				pMin[i] += f;
				pMax[i] += e;
			}
		}
	}
}

BBox::~BBox()
{
}
const Point3f BBox::getMidPoint() const
{
	return (pMax + pMin) / 2.0;
}

const Vector3f BBox::getDiagnal() const
{
	return pMax - pMin;
}

bool BBox::isInside(const Point3f &pos) const
{
	if (pos.x < pMin.x || pos.x > pMax.x)
	{
		return false;
	}
	if (pos.y < pMin.y || pos.y > pMax.y)
	{
		return false;
	}
	if (pos.z < pMin.z || pos.z > pMax.z)
	{
		return false;
	}
	return true;
}

void BBox::expand(Float delta)
{
	pMin -= Vector3f(delta, delta, delta);
	pMax += Vector3f(delta, delta, delta);
}
bool BBox::intersectP(const Ray& inRay, Float *hitt0, Float *hitt1) const
{
	Float t0 = inRay.tmin, t1 = inRay.tmax;
	//Float t0 = 0, t1 = INFINITY;
	for (int i = 0; i < 3; i++)
	{
		Float tNear = (pMin[i] - inRay.o[i]) / inRay.d[i];
		Float tFar = (pMax[i] - inRay.o[i]) / inRay.d[i];
		if (tNear > tFar)
		{
			swap(tNear, tFar);
		}
		t0 = tNear > t0 ? tNear : t0;
		t1 = tFar < t1 ? tFar : t1;
		if (t0 > t1)
		{
			return false;
		}
	}
	if (hitt0)
	{
		*hitt0 = t0;
	}
	if (hitt1)
	{
		*hitt1 = t1;
	}
	return true;
}

Float BBox::sqDist(const Point3f &p) const
{
	Float sqD = 0;

	for (int i = 0; i < 3; i++)
	{
		Float v = p[i];
		if (v < this->pMin[i])
		{
			sqD += sqr(this->pMin[i] - v);
		}
		if (v > this->pMax[i])
		{
			sqD += sqr(this->pMax[i] - v);
		}
	}
	return sqD;
}

/*
bool BBox::overlaps(const BBox& box) const
{
	bool over = true;
	if ()
	{
	}
	for (int i = 0; i < 3; i++)
	{
		over &= (pMax[i] >= box.pMin[i]) && (pMin[i] <= box.pMax[i]);
	}
	return over;	
}*/
bool overlaps(const BBox &box0, const BBox& box1)
{
	if (box0.pMax.x < box1.pMin.x || box0.pMin.x > box1.pMax.x)
	{
		return false;
	}
	if (box0.pMax.y < box1.pMin.y || box0.pMin.y > box1.pMax.y)
	{
		return false;
	}
	if (box0.pMax.z < box1.pMin.z || box0.pMin.z > box1.pMax.z)
	{
		return false;
	}
	return true;
}

void BBox::Union(const Point3f &p)
{
	for (int i = 0; i < 3; i++)
	{
		this->pMin[i] = min(this->pMin[i], p[i]);
		this->pMax[i] = max(this->pMax[i], p[i]);
	}
}
void BBox::Union(const BBox& box)
{
	for (int i = 0; i < 3; i++)
	{
		this->pMin[i] = min(this->pMin[i], box.pMin[i]);
		this->pMax[i] = max(this->pMax[i], box.pMax[i]);
	}
}
BBox Union(const BBox& box, const Point3f &p)
{
	BBox ret = box;
	for (int i = 0; i < 3; i++)
	{
		ret.pMin[i] = min(box.pMin[i], p[i]);
		ret.pMax[i] = max(box.pMax[i], p[i]);
	}
	return ret;
}
BBox Union(const BBox& box1, const BBox& box2)
{
	BBox ret;
	for (int i = 0; i < 3; i++)
	{
		ret.pMin[i] = min(box1.pMin[i], box2.pMin[i]);
		ret.pMax[i] = max(box1.pMax[i], box2.pMax[i]);
	}
	return ret;
}

/*
bool touch(const BBox &box0, const BBox& box1)
{
	if (box0.pMax.x < box1.pMin.x || box0.pMin.x > box1.pMax.x)
	{
		return false;
	}
	if (box0.pMin.y > box1.pMax.y || box0.pMax.y < box1.pMin.y)
	{
		return false;
	}
	if (box0.pMax.z < box1.pMin.z || box0.pMin.z > box1.pMax.z)
	{
		return false;
	}
	return true;
}*/

int BBox::maxExtent() const
{
	Vector3f diag = pMax - pMin;
	if (diag.x > diag.y && diag.x > diag.z)
	{
		return 0;
	} 
	else if (diag.y > diag.z)
	{
		return 1;
	} 
	else
	{
		return 2;
	}
}
Float BBox::surfaceArea() const
{
	Vector3f d = pMax - pMin;
	return 2.0 * (d[0] * d[1] + d[0] * d[2] + d[1] * d[2]);
}

void BBox::printInfo() const
{
	cout << "min point:" << pMin << endl;
	cout << "max point:" << pMax << endl;
	cout << "size:" << (pMax - pMin) << endl;
}
