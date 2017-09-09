#pragma once

#include "Accel/Bounds.h"
#include "Geometry/Primitive.h"
#include "Geometry/Intersection.h"

namespace Kaguya
{

struct BoundEdge
{
	Float t;//length of the edge of a bounding box
	int primNum;//object index
	enum { START, END } type;

	BoundEdge() {}
	BoundEdge(Float len, int pn, bool starting) : t(len), primNum(pn)
	{
		type = starting ? START : END;
	}
	bool operator<(const BoundEdge &b) const
	{
		if (t == b.t)
		{
			if (primNum == b.primNum)
			{
				return (int)b.type > (int)type;
			}
			return (int)b.type < (int)type;//I changed the order other than pbrt did.
		}
		else
		{
			return t < b.t;
		}
	}
};

struct KdAccelNode//Node class
{
	Bounds3f bbox;
	//std::vector<Shape*> primitives;
	int flags;//index of split axes
	std::vector<int> primIndex;//only for leaf
	KdAccelNode* belowNode;
	KdAccelNode* aboveNode;//only for interior
	Float split;//split position only for interior

				//void initLeaf(int* prims, int np);//primitive indices array
	void initLeaf(std::vector<int> &prims);//primitive indices array
	void initInterior(int axis, Float s);

	bool isLeaf() const;
	int axis() const;
	~KdAccelNode();
	//KdAccelNode* build(std::vector<Shape*> &tris, int depth) const;
	// 	bool hit(KdAccelNode* node, const Ray &inRay,
	// 		Float &t, Float &tmin, Intersection* isec) const;

	void printInfo() const;
};
class KdTreeAccel//Tree class
{
public:
	KdTreeAccel(const std::vector<Primitive*> &prims,
				int md = -1, int mp = 3, Float eb = 0.5);
	~KdTreeAccel();
	bool intersectP(const Ray &inRay) const;
	bool intersect(const Ray &inRay,
				   Intersection* isec,
				   Float* tHit, Float* rayEpsilon) const;
	bool intersect(const Ray &inRay,
				   Intersection* isec,
				   const KdAccelNode* node,
				   Float* tHit, Float* rayEpsilon) const;
	bool inLeaf(const Point3f &pos) const;
	bool inLeaf(const Point3f &pos, const KdAccelNode* node) const;

	//update tree?
	void update();

	void printInfo() const;
	void printNode(KdAccelNode* node) const;

private:
	void buildTree(KdAccelNode* node, const Bounds3f &bound,
				   std::vector<int> &prims,
				   int depth, BoundEdge* edges[3]);

private:
	//int isectCost, traversalCost,
	int maxDepth, maxPrims;
	Float emptyBonus;
	std::vector<Primitive*> primitives;
	KdAccelNode* root;
	Bounds3f treeBound;

	friend class RasterizedVolume;

};

struct KdToDo
{
	const KdAccelNode* node;
	Float tmin, tmax;
};

}
