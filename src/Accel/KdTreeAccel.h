#pragma once
#ifndef __KDTREEACCEL__
#define __KDTREEACCEL__


#ifndef KAGUYA_DOUBLE_AS_FLOAT
#define KAGUYA_DOUBLE_AS_FLOAT
#endif // !KAGUYA_DOUBLE_AS_FLOAT
#include "Core/Kaguya.h"
#include "Dynamics/Collision.h"
#include "Geometry/Shape.h"
#include "Geometry/DifferentialGeometry.h"

struct BoundEdge
{
	Float t;//length of the edge of a bounding box
	int primNum;//object index
	enum {	START, END } type;

	BoundEdge(){}
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
	BBox bbox;
	//vector<Shape*> primitives;
	int flags;//index of split axes
	vector<int> primIndex;//only for leaf
	KdAccelNode *belowNode, *aboveNode;//only for interior
	Float split;//split position only for interior


	//void initLeaf(int *prims, int np);//primitive indices array
	void initLeaf(vector<int> &prims);//primitive indices array
	void initInterior(int axis, Float s);

	bool isLeaf() const;
	int axis() const;
	~KdAccelNode();
	//KdAccelNode* build(vector<Shape*> &tris, int depth) const;
// 	bool hit(KdAccelNode* node, const Ray& inRay,
// 		Float& t, Float& tmin, DifferentialGeometry *queryPoint) const;

	void printInfo() const;
};
class KdTreeAccel//Tree class
{
public:
	KdTreeAccel(const vector<Shape*> &prims, int md = -1, int mp = 3,
		Float eb = 0.5);
	~KdTreeAccel();
	bool hitP(const Ray &inRay) const;
	bool hit(const Ray &inRay, DifferentialGeometry *queryPoint,
		Float *tHit, Float *rayEpsilon) const;
	bool hit(const Ray &inRay, DifferentialGeometry *queryPoint, const KdAccelNode *node,
		Float *tHit, Float *rayEpsilon) const;
	bool inLeaf(const Point3D& pos) const;
	bool inLeaf(const Point3D& pos, const KdAccelNode *node) const;

	bool collide(const Shape* inObj, const BBox &worldbound,
		DifferentialGeometry *queryPoint = nullptr, Float *tHit = nullptr) const;
	bool collide(const Shape* inObj, const BBox &worldbound,
		DifferentialGeometry *queryPoint,
		const KdAccelNode *node, Float *tHit) const;
	//update tree?
	void update();

	void printInfo() const;
	void printNode(KdAccelNode* node) const;

private:
	friend class RasterizedVolume;
	friend class Collision;
private:
	//int isectCost, traversalCost, 
	int maxDepth, maxPrims;
	Float emptyBonus;
	vector<Shape*> primitives;
	KdAccelNode *root;
	BBox treeBound;

	void buildTree(KdAccelNode *node, const BBox &bound, vector<int> &prims,
		int depth, BoundEdge *edges[3]);

};

struct KdToDo
{
	const KdAccelNode* node;
	Float tmin, tmax;
};


#endif // __KDTREEACCEL__