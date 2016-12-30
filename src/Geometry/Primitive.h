//
//  simpleGeomtry.h
//
//  Created by Shenyao Ke on 1/26/15.
//  Copyright (c) 2015 AKIKA. All rights reserved.
//
#pragma once

#include "Math/Geometry.h"
#include "Accel/Bounds.h"
#include "Image/ColorData.h"
#include "Math/Transform.h"
#include "Geometry/DifferentialGeometry.h"
#include "Shading/BxDF.h"

const Float reCE = 5e-8;//ray epsilon coefficiency
const Float FloatEps = std::numeric_limits<Float>::epsilon();//Distance epsilon coefficiency
/************************************************************************/
/* Basic Shape Function Definition                                      */
/************************************************************************/
namespace Kaguya
{

class ParamatricGeometry;
class Mesh;
class PolyMesh;
class SubdMesh;
class Curve;

enum class PrimitiveType
{
    PARAMATRIC_SURFACE,
    POLYGONAL_MESH,
    SUBDIVISION_MESH,
    CURVE,
    INSTANCE
};

static const uint32_t sInvalidGeomID = (uint32_t)(-1);

class Primitive
{
public:
	Primitive(const Transform* o2w = nullptr)
		: shapeID(nextshapeID++)
        , mObjectToWorld(o2w)
    {
	}
    virtual ~Primitive() {}

	virtual void bounding() = 0;

    const Bounds3f &getWorldBounding() const { return mObjBound; }

	virtual bool intersectP(const Ray &inRay) const;

	virtual bool intersect(const Ray &inRay,
                           DifferentialGeometry* dg,
		                   Float* tHit,
                           Float* rayEpsilon) const = 0;

	virtual void postIntersect(const Ray &inRay,
		                       DifferentialGeometry* dg) const = 0;

	// Surface Area
    virtual Float area() const { return 0; }

	// Probability Density Function
    virtual Float pdf() const { return 1; }

	// Shading
    virtual void getShadingGeometry(const Transform &obj2world,
                                    DifferentialGeometry* dg) const {}

    virtual bool isInside(const Point3f &pPos) const { return false; }

    virtual void printInfo(const std::string &msg = "") const;

    virtual PrimitiveType primitiveType() const = 0;

    uint32_t getShapeID() const
    {
        return shapeID;
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
    static uint32_t         nextshapeID;
    const uint32_t          shapeID;

    std::string             mPrimName;

	const Transform*        mObjectToWorld;
	Bounds3f                mObjBound;

    std::shared_ptr<BxDF>   bxdf;
};

}
