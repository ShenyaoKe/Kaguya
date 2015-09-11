#pragma once
#ifndef __KDTREEACCEL__
#define __KDTREEACCEL__

#include "Core/rtdef.h"
#include "Geometry/Shape.h"
#include "Geometry/DifferentialGeometry.h"

struct BoundEdge
{
	double t;//length of the edge of a bounding box
	int primNum;//object index
	enum {	START, END } type;

	BoundEdge(){}
	BoundEdge(double len, int pn, bool starting) : t(len), primNum(pn)
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
	double split;//split position only for interior


	//void initLeaf(int *prims, int np);//primitive indices array
	void initLeaf(vector<int> &prims);//primitive indices array
	void initInterior(int axis, double s);

	bool isLeaf() const;
	int axis() const;
	~KdAccelNode();
	//KdAccelNode* build(vector<Shape*> &tris, int depth) const;
// 	bool hit(KdAccelNode* node, const Ray& inRay,
// 		double& t, double& tmin, DifferentialGeometry *queryPoint) const;

	void printInfo() const;
};
class KdTreeAccel//Tree class
{
public:
	KdTreeAccel(const vector<Shape*> &prims, int md = -1, int mp = 3,
		double eb = 0.5);
	~KdTreeAccel();
	bool hitP(const Ray &inRay) const;
	bool hit(const Ray &inRay, DifferentialGeometry *queryPoint,
		double *tHit, double *rayEpsilon) const;
	bool hit(const Ray &inRay, DifferentialGeometry *queryPoint, const KdAccelNode *node,
		double *tHit, double *rayEpsilon) const;
	//update tree?
	void update();

	void printInfo() const;
	void printNode(KdAccelNode* node) const;

private:
	//int isectCost, traversalCost, 
	int maxDepth, maxPrims;
	double emptyBonus;
	vector<Shape*> primitives;
	KdAccelNode *root;
	BBox treeBound;

	void buildTree(KdAccelNode *node, const BBox &bound, vector<int> &prims,
		int depth, BoundEdge *edges[3]);
};

struct KdToDo
{
	const KdAccelNode* node;
	double tmin, tmax;
};


#endif // __KDTREEACCEL__