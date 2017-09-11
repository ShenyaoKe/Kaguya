//
//  simpleGeomtry.h
//
//  Created by Shenyao Ke on 1/26/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once

#include "Math/Vector.h"
#include "Accel/Bounds.h"
#include "Image/ColorData.h"
#include "Math/Transform.h"
#include "Geometry/Intersection.h"
#include "Shading/BxDF.h"
#include "PrimitiveAttribute.h"

const Float reCE = 5e-8;//ray epsilon coefficient

//Distance epsilon coefficient
const Float FloatEps = std::numeric_limits<Float>::epsilon();

/************************************************************************/
/* Basic Geometry Class Definition                                      */
/************************************************************************/
namespace Kaguya
{

class ParamatricGeometry;
class Mesh;
class PolyMesh;
class SubdMesh;
class Curve;

struct VertexBufferTrait
{
	const void* data = nullptr;
	uint32_t    count = 0;// element count
	uint32_t    size = 0;// data size in bytes
	uint32_t    offset = 0;// actual data offset in bytes
	uint32_t    stride = 0;// element size in bytes
};
struct AttributeTextureTrait
{
	const void*     attriData = nullptr;
	const void*     attriIndex = nullptr;
	uint32_t        dataSize = 0; // size of data in bytes
	uint32_t        indexSize = 0;
};

enum class GPURenderType : uint8_t
{
	CURVE,
	TRIANGLE,
	QUAD,
};

// Used for GPU render
struct RenderBufferTrait
{
	VertexBufferTrait       vertex;
	VertexBufferTrait       index;
	AttributeTextureTrait   texcoords;
	AttributeTextureTrait   normal;
	AttributeType           texAttriType = AttributeType::UNDEFINED;
	AttributeType           normAttriType = AttributeType::UNDEFINED;
	GPURenderType           renderType;
};

enum class GeometryType
{
	PARAMATRIC_SURFACE,
	POLYGONAL_MESH,
	SUBDIVISION_MESH,
	CURVE,
	INSTANCE
};

static const uint32_t sInvalidGeomID = (uint32_t)(-1);

class Geometry
{
public:
	Geometry(const Transform* o2w = nullptr)
		: mGeomID(sNextGeomID++)
		, mObjectToWorld(o2w)
	{
	}
	virtual ~Geometry() {}

	virtual void bounding() = 0;

	const Bounds3f &getWorldBounding() const { return mObjBound; }

	virtual bool intersectP(const Ray &inRay) const;

	virtual bool intersect(const Ray &inRay,
						   Intersection* isec,
						   Float* tHit,
						   Float* rayEpsilon) const = 0;

	virtual void postIntersect(const Ray &inRay,
							   Intersection* isec) const = 0;

	// Surface Area
	virtual Float area() const { return 0; }

	// Probability Density Function
	virtual Float pdf() const { return 1; }

	// Shading
	virtual void getShadingGeometry(const Transform &obj2world,
									Intersection* isec) const
	{
	}

	virtual bool isInside(const Point3f &pPos) const { return false; }

	virtual void printInfo(const std::string &msg = "") const;

	virtual GeometryType primitiveType() const = 0;

	virtual void getRenderBuffer(RenderBufferTrait* trait) const = 0;

	uint32_t getShapeID() const
	{
		return mGeomID;
	}

	void setName(const std::string &primName)
	{
		mPrimName = primName;
	}

	void setTransform(const Transform* xform)
	{
		mObjectToWorld = xform;
	}

	const Matrix4x4 &objectToWorld() const
	{
		return mObjectToWorld->getMat();
	}

	const Matrix4x4 &worldToObject() const
	{
		return mObjectToWorld->getInvMat();
	}

protected:
	static uint32_t         sNextGeomID;
	const uint32_t          mGeomID;

	std::string             mPrimName;

	const Transform*        mObjectToWorld;
	Bounds3f                mObjBound;

	std::shared_ptr<BxDF>   bxdf;
};

}
