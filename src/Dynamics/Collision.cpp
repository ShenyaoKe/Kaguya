#include "Dynamics/Collision.h"
#include "Accel/KdTreeAccel.h"
bool Collision::collideP(const BBox &box0, const BBox &box1)
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
bool Collision::collideP(const BBox &box0, const Matrix4D &b0mat, const BBox &box1)
{
	BBox newbox(box0, b0mat);
	return collideP(newbox, box1);
}
bool Collision::collideP(
	const BBox &box0, const Matrix4D &b0mat,
	const BBox &box1, const Matrix4D &b1mat)
{
	BBox newbox0(box0, b0mat);
	BBox newbox1(box1, b1mat);
	return collideP(newbox0, newbox1);
}
bool Collision::collideP(const geoSphere &sphere, const BBox &box)
{
	return box.sqDist(sphere.getCenter()) <= sqr(sphere.getRadius());
}
bool Collision::collideP(const geoSphere &sphere, const Triangle &triangle)
{
	Point3D p = triangle.closestPoint(sphere.getCenter());
	return (p - sphere.getCenter()).getLenSq() <= sqr(sphere.getRadius());
}
bool Collision::collideP(const geoSphere &sph0, const geoSphere &sph1)
{
	Float sqDist = (sph0.getCenter() - sph1.getCenter()).getLenSq();
	return sqDist <= sqr(sph0.getRadius() + sph1.getRadius());
}
bool Collision::collide(const Vector3D &prePos, const Vector3D &curPos,
	const KdTreeAccel *tree, DifferentialGeometry* queryPoint,
	Float *tHit, Float* hitEpsilon)
{
	Ray ray = Ray(prePos, curPos - prePos);
	ray.tmax = (curPos - prePos).getLength();
	*tHit = ray.tmax;
	return tree->hit(ray, queryPoint, tHit, hitEpsilon);
}
bool Collision::collide(const BBox &targetBound,
	const Matrix4D &treeMat, const KdTreeAccel* tree,
	BBox* &collisionBound, vector<Point3D*> &primpts)
{
	if (!collideP(tree->treeBound, treeMat, targetBound))
	{
		return false;
	}
	collide(targetBound, treeMat, tree->root, tree, collisionBound, primpts);
}

bool Collision::collide(const BBox &targetBound, const Matrix4D &treeMat,
	const KdAccelNode* treeNode, const KdTreeAccel* tree,
	BBox* &collisionBound, vector<Point3D*> &primpts)
{
	if (!collideP(treeNode->bbox, treeMat, targetBound))
	{
		return false;
	}
	// If collideP check if node is leaf
	if (!treeNode->isLeaf())
	{
		auto belowNode = treeNode->belowNode;
		auto aboveNode = treeNode->aboveNode;
		bool res = collide(targetBound, treeMat, belowNode, tree, collisionBound, primpts);
		//if (!res)
		{
			res |= collide(targetBound, treeMat, aboveNode, tree, collisionBound, primpts);
		}
		return res;
	}
	else// If node is leaf
	{
		/*collisionBound = new BBox(treeNode->bbox);
		return true;*/
		BBox* imBox = new BBox;
		treeNode->primIndex;
		vector<Point3D*> curPrimPts;
		for (auto idx : treeNode->primIndex)
		{
			auto prim = dynamic_cast<Triangle*>(tree->primitives[idx]);
			for (auto pt : prim->p)
			{
				auto wPt = new Point3D((treeMat * Vector4D(*pt, 1.0)).toVector3D());
				imBox->Union(*wPt);
				curPrimPts.push_back(wPt);
				//primpts.push_back(wPt);
			}
		}
		if (collideP(*imBox, targetBound))
		{
			collisionBound = imBox;
			//primpts.push_back(curPrimPts);
			primpts.insert(primpts.end(), curPrimPts.begin(), curPrimPts.end());

			return true;
		}
		for (auto pt : curPrimPts)
		{
			delete pt;
		}
		curPrimPts.clear();
		delete imBox;
		return false;
	}
}
