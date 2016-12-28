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

const Float reCE = 5e-8;//ray epsilon coefficiency
const Float FloatEps = std::numeric_limits<Float>::epsilon();//Distance epsilon coefficiency
/************************************************************************/
/* Basic Shape Function Definition                                      */
/************************************************************************/
namespace Kaguya
{

class Primitive
{
public:
	Primitive(const Transform* o2w = nullptr)
		: shapeID(nextshapeID++)
        , ObjectToWorld(o2w)
        , UV_Mapping(nullptr), normalMap(nullptr)
    {
	}
    virtual ~Primitive() {}

	virtual void bounding() = 0;
    virtual Bounds3f getWorldBounding() const { return ObjBound; }
	virtual void refine(vector<Primitive*> &refined) {}
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
    virtual void assignTextureMapping(TextureMapping* &mapping) {
        UV_Mapping = mapping;
    }
    virtual void assignNormalMap(Texture* nMap) {
        normalMap = nMap;
    }

    virtual void printInfo() const;

public:
    static uint32_t nextshapeID;
    const uint32_t shapeID;

	const Transform* ObjectToWorld;
	Bounds3f ObjBound;

	TextureMapping* UV_Mapping;
	Texture* normalMap;
};

}
