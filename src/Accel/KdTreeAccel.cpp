#include "Accel/KdTreeAccel.h"

static int leafCount(0);
char axisChar[4] = { 'X', 'Y', 'Z', 'L' };
KdTreeAccel::KdTreeAccel(const vector<Shape*> &prims, int md, int mp,
	Float eb) : maxDepth(md), maxPrims(mp), emptyBonus(eb)
{
	int np = prims.size();
	primitives = prims;
	//Initialize tree depth
	if (maxDepth <= 0)
	{
		maxDepth = Round2Int(8 + 1.3 * Log2Int(static_cast<Float>(np)));
	}
	// If no node in vector, terminate initialization
	if (np == 0)
	{
		root = nullptr;
		return;
	}
	//Initialize bouding box for all primitives in stack
	for (int i = 0; i < np; ++i)
	{
		treeBound.Union(prims[i]->ObjBound);
	}
	//Allocate bound edge info
	BoundEdge *edges[3];
	for (int i = 0; i < 3; ++i)
	{
		edges[i] = new BoundEdge[np * 2];
	}
	//Create stack to record primitive indices
	vector<int> primNum(np);
	for (int i = 0; i < np; ++i)
	{
		primNum[i] = i;
	}
	//Start recursively build the tree
	root = new KdAccelNode;
	buildTree(root, treeBound, primNum, maxDepth, edges);

	//Clean data
	for (int i = 0; i < 3; ++i)
	{
		delete[] edges[i];
		edges[i] = nullptr;
	}
}

void KdTreeAccel::buildTree(KdAccelNode *node, const BBox &bound, vector<int> &prims,
	int depth, BoundEdge *edges[3])
{
	node->bbox = bound;
	int np = prims.size();
	//If not enough primitives or reach max depth of a tree
	//Create a leaf node
	if (np <= maxPrims || depth == 0)
	{
		node->initLeaf(prims);
		return;
	}
	//Interior node parameters
	int bestAxis = -1, bestOffest = -1;//Split axis and split index*2
	Float bestCost = INFINITY;
	Float oldCost = np;
	Float totalSA = bound.surfaceArea();
	Float invTotalSA = 1.0 / totalSA;
	Vector3f bbDiag = bound.pMax - bound.pMin;

	//choose max extension axis of bounding box
	int axis = bound.maxExtent();//axis is the longest edge of bounding box
	int retries = 0;
RETRY_SPLIT:
	//
	//Calculate surface cost
	for (int i = 0; i < np; ++i)
	{
		int prmIdx = prims[i];
		const BBox tmpBox = primitives[prmIdx]->ObjBound;
		edges[axis][i * 2] = BoundEdge(tmpBox.pMin[axis], prmIdx, true);
		edges[axis][i * 2 + 1] = BoundEdge(tmpBox.pMax[axis], prmIdx, false);
	}

	sort(&edges[axis][0], &edges[axis][2 * np]);
	int nBelow(0), nAbove(np);
	for (int i = 0; i < 2 * np; ++i)
	{
		if (edges[axis][i].type == BoundEdge::END)
		{
			--nAbove;
		}
		Float edget = edges[axis][i].t;

		//when t is in between the boundary
		if (edget > bound.pMin[axis] && edget < bound.pMax[axis])
		{
			//Compute cost for split at ith edge
			//get other two axes
			int axis0 = (axis + 1) % 3, axis1 = (axis + 2) % 3;
			Float belowSA = 2 * (bbDiag[axis0] * bbDiag[axis1] +
				(edget - bound.pMin[axis]) *
				(bbDiag[axis0] + bbDiag[axis1]));
			Float aboveSA = 2 * (bbDiag[axis0] * bbDiag[axis1] +
				(bound.pMax[axis] - edget) *
				(bbDiag[axis0] + bbDiag[axis1]));
			Float pBelow = belowSA * invTotalSA;
			Float pAbove = aboveSA * invTotalSA;
			Float eb = (nAbove == 0 || nBelow == 0) ? emptyBonus : 0;
			Float cost = (1.0 - eb) * (pBelow * nBelow + pAbove * nAbove);

			//cout << "cost at i:" << i << " is " << cost << endl;
			if (cost < bestCost)
			{
				bestCost = cost;
				bestAxis = axis;
				bestOffest = i;// edges[axis][i].type ? i + 1 : i;//When end use next node
			}
		}
		if (edges[axis][i].type == BoundEdge::START)
		{
			++nBelow;
		}
	}
	
	if (retries < 2)//(bestAxis == -1 && retries < 2)//
	{
		++retries;
		axis = (axis + 1) % 3;
		goto RETRY_SPLIT;
	}
	//if no good split, init as leaf
	if (bestAxis == -1)
	{
		node->initLeaf(prims);
		return;
	}

	//recursively build subtree
	vector<int> primsBelow;
	vector<int> primsAbove;
	//Store indices of below primitives
	for (int i = 0; i < bestOffest; ++i)
	{
		if (edges[bestAxis][i].type == BoundEdge::START)
		{
			primsBelow.push_back(edges[bestAxis][i].primNum);
		}
	}
	//Store indices of above primitives
	for (int i = bestOffest + 1; i < np * 2; ++i)
	{
		if (edges[bestAxis][i].type == BoundEdge::END)
		{
			primsAbove.push_back(edges[bestAxis][i].primNum);
		}
	}
	Float tsplit = edges[bestAxis][bestOffest].t;
	node->initInterior(bestAxis, tsplit);

	/*for (int i = 0; i < 2 * np; ++i)
	{
		cout << edges[bestAxis][i].primNum << ": "
			<< edges[bestAxis][i].t << "; Start/End: " << edges[bestAxis][i].type << endl;
	}
	cout << "Split on axis " << axisChar[bestAxis] << " at " << tsplit << endl;
	cout << "Best offset: " << bestOffest << endl;
	cout << "Prims above: ";
	for (int i = 0; i < primsAbove.size(); ++i)
	{
		cout << primsAbove[i] << " ";
	}cout << endl;
	cout << "Prims below: ";
	for (int i = 0; i < primsBelow.size(); ++i)
	{
		cout << primsBelow[i] << " ";
	}cout << endl;
	cout << "////////////////////////" << endl;*/
	BBox belowBound = bound, aboveBound = bound;
	belowBound.pMax[bestAxis] = aboveBound.pMin[bestAxis] = tsplit;

	buildTree(node->belowNode, belowBound, primsBelow, depth - 1, edges);
	buildTree(node->aboveNode, aboveBound, primsAbove, depth - 1, edges);

}
bool KdTreeAccel::intersect(const Ray &inRay, DifferentialGeometry* queryPoint,
	Float *tHit, Float *rayEpsilon) const
{
	return intersect(inRay, queryPoint, root, tHit, rayEpsilon);
}
bool KdTreeAccel::intersect(const Ray &inRay, DifferentialGeometry* queryPoint, const KdAccelNode *node,
	Float *tHit, Float *rayEpsilon) const
{
	//Compute initial parametric range of ray inside kd-tree extent
	Float tmin, tmax, rayEp;//temprary DifferentialGeometry result
	if (!node->bbox.intersectP(inRay, &tmin, &tmax))
	{
		return false;
	}

	//prepare to traversal kd-tree for ray
	Vector3f invDir(1.0 / inRay.d.x, 1.0 / inRay.d.y, 1.0 / inRay.d.z);

	//Traversal kd-tree node in order of ray
	bool isHit = false;
	if (node != nullptr)
	{
		//if hit outside the box, think it's used for later use
		if (inRay.tmax < tmin)
		{
			return isHit;
		}
		if (node->isLeaf())
		{
			DifferentialGeometry* tmpQuery = new DifferentialGeometry();
			Float hitDist;
			for (int i = 0; i < node->primIndex.size(); ++i)
			{
				int idx = node->primIndex[i];

				if (primitives[idx]->intersectP(inRay))
				{
					if (primitives[idx]->intersect(inRay, tmpQuery, &hitDist, &rayEp))
					{
						if (hitDist < *tHit && inRange(hitDist, tmin, tmax))
						{
							*queryPoint = *tmpQuery;
							*tHit = hitDist;
							*rayEpsilon = rayEp;
							queryPoint->shape = primitives[idx];
							isHit = true;
						}
					}
				}
			}
			delete tmpQuery;
		}
		else//if hit interior node
		{
			/*process interior node*/
			//calculate parametric distance from ray to split plane
			int axis = node->flags;
			Float tsplit = (node->split - inRay.o[axis]) * invDir[axis];

			//get children node for ray
			const KdAccelNode *nearChild, *farChild;
			bool belowFisrt = ((inRay.o[axis] < node->split) ||
				(inRay.o[axis] == node->split && inRay.d[axis] < 0));
			if (belowFisrt)
			{
				nearChild = node->belowNode;
				farChild = node->aboveNode;
			}
			else
			{
				nearChild = node->aboveNode;
				farChild = node->belowNode;
			}
			if (tsplit > tmax || tsplit <= 0)
			{
				isHit = intersect(inRay, queryPoint, nearChild, tHit, rayEpsilon);
			}
			else if (tsplit < tmin)
			{
				isHit = intersect(inRay, queryPoint, farChild, tHit, rayEpsilon);
			}
			else
			{
				isHit = intersect(inRay, queryPoint, nearChild, tHit, rayEpsilon);
				if (!isHit)
				{
					isHit = intersect(inRay, queryPoint, farChild, tHit, rayEpsilon);
				}
			}
// 			nearChild = nullptr;
// 			farChild = nullptr;
		}
	}
	return isHit;
}
KdTreeAccel::~KdTreeAccel()
{
	primitives.clear();
}

void KdTreeAccel::printInfo() const
{
	if (root->flags == 3)
	{
		root->printInfo();
	}
	else
	{
		root->belowNode->printInfo();
		root->aboveNode->printInfo();
	}
}

void KdTreeAccel::printNode(KdAccelNode* node) const
{
	if (node->flags == 3)
	{
		node->printInfo();
	}
	else
	{
		printNode(node->belowNode);
		printNode(node->aboveNode);
	}
}

bool KdTreeAccel::intersectP(const Ray &inRay) const
{
	return treeBound.intersectP(inRay);
}

void KdTreeAccel::update()
{
	int np = primitives.size();
	//Initialize bouding box for all primitives in stack
	treeBound = BBox();
	for (int i = 0; i < np; ++i)
	{
		treeBound.Union(primitives[i]->ObjBound);
	}
	//Allocate bound edge info
	BoundEdge *edges[3];
	for (int i = 0; i < 3; ++i)
	{
		edges[i] = new BoundEdge[np * 2];
	}
	//Create stack to record primitive indices
	vector<int> primNum(np);
	for (int i = 0; i < np; ++i)
	{
		primNum[i] = i;
	}
	//Start recursively build the tree
	buildTree(root, treeBound, primNum, maxDepth, edges);

	//Clean data
	for (int i = 0; i < 3; ++i)
	{
		delete[] edges[i];
//		edges[i] = nullptr;
	}
}

bool KdTreeAccel::inLeaf(const Point3f &pos) const
{
	/*if (!treeBound.isInside(pos))
	{
		return false;
	}*/
	return inLeaf(pos, root);
}

bool KdTreeAccel::inLeaf(const Point3f &pos, const KdAccelNode *node) const
{
	bool isInLeaf = false;
	if (node!=nullptr)
	{
		if (!node->bbox.isInside(pos))
		{
			return false;
		}
		if (node->isLeaf())
		{
			return true;
		}

		isInLeaf = this->inLeaf(pos, node->belowNode);
		if (!isInLeaf)
		{
			isInLeaf = this->inLeaf(pos, node->aboveNode);
		}
	}
	return isInLeaf;
}

void KdAccelNode::initLeaf(vector<int> &prims)
{
	//int np = prims.size();
	flags = 3;

	primIndex = prims;
	belowNode = nullptr;
	aboveNode = nullptr;
}
void KdAccelNode::initInterior(int axis, Float s)
{
	flags = axis;
	split = s;
	belowNode = new KdAccelNode;
	aboveNode = new KdAccelNode;
}

void KdAccelNode::printInfo() const
{
	if (flags == 3)
	{
		
 		cout << "Leaf primitives: ";
		if (primIndex.size() == 0)
		{
			cout << "no primitive in this leaf\n"\
				"Pmin:\t" << (bbox.pMax - bbox.pMin) << endl;
		}
		for (int i = 0; i < primIndex.size(); ++i)
		{
			cout << primIndex[i] << "\t";
		}
		cout << endl;
	}
	else
	{
		this->belowNode->printInfo();
		this->aboveNode->printInfo();
	}
}

bool KdAccelNode::isLeaf() const
{
	return (flags == 3);
}

KdAccelNode::~KdAccelNode()
{
	primIndex.clear();
	delete belowNode;
//	belowNode = nullptr;
	delete aboveNode;
//	aboveNode = nullptr;
}