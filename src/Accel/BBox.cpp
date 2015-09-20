#include "Accel/BBox.h"

BBox::BBox()
{
	pMin = Point3D(INFINITY, INFINITY, INFINITY);
	pMax = Point3D(-INFINITY, -INFINITY, -INFINITY);
}


BBox::BBox(const Point3D& p) :pMin(p), pMax(p)
{
}
BBox::BBox(const Point3D& p1, const Point3D& p2)
{
	pMin = Point3D(min(p1.x, p2.x), min(p1.y, p2.y), min(p1.z, p2.z));
	pMax = Point3D(max(p1.x, p2.x), max(p1.y, p2.y), max(p1.z, p2.z));
}
BBox::~BBox()
{
}
const Point3D BBox::getMidPoint() const
{
	return (pMax + pMin) / 2.0;
}
void BBox::expand(Float delta)
{
	pMin -= Point3D(delta, delta, delta);
	pMax += Point3D(delta, delta, delta);
}
bool BBox::intersectP(const Ray& inRay, Float *hitt0, Float *hitt1) const
{
	Float t0 = inRay.tmin, t1 = inRay.tmax;
	//Float t0 = 0, t1 = INFINITY;
	for (int i = 0; i < 3; i++)
	{
		Float tNear = (pMin[i] - inRay.pos[i]) / inRay.dir[i];
		Float tFar = (pMax[i] - inRay.pos[i]) / inRay.dir[i];
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

Float BBox::sqDist(const Point3D &p) const
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
	if (box0.pMax.y < box1.pMin.y || box0.pMin.y > box1.pMax.y);
	{
		return false;
	}
	if (box0.pMax.z < box1.pMin.z || box0.pMin.z > box1.pMax.z)
	{
		return false;
	}
	return true;
}

void BBox::Union(const Point3D& p)
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
BBox Union(const BBox& box, const Point3D& p)
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
int BBox::maxExtent() const
{
	Vector3D diag = pMax - pMin;
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
	Vector3D d = pMax - pMin;
	return 2.0 * (d[0] * d[1] + d[0] * d[2] + d[1] * d[2]);
}

void BBox::printInfo() const
{
	cout << "min point:"; pMin.printInfo();
	cout << "max point:"; pMax.printInfo();
	cout << "size:"; (pMax - pMin).printInfo();
}
