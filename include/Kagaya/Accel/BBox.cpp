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
void BBox::expand(double delta)
{
	pMin -= Point3D(delta, delta, delta);
	pMax += Point3D(delta, delta, delta);
}
bool BBox::intersectP(const Ray& inRay, double *hitt0, double *hitt1) const
{
	double t0 = inRay.tmin, t1 = inRay.tmax;
	//double t0 = 0, t1 = INFINITY;
	for (int i = 0; i < 3; i++)
	{
		double tNear = (pMin[i] - inRay.pos[i]) / inRay.dir[i];
		double tFar = (pMax[i] - inRay.pos[i]) / inRay.dir[i];
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

bool BBox::overlaps(const BBox& box) const
{
	bool over = true;
	for (int i = 0; i < 3; i++)
	{
		over &= (pMax[i] >= box.pMin[i]) && (pMin[i] <= box.pMax[i]);
	}
	return over;	
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
double BBox::surfaceArea() const
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
