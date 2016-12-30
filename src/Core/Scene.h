#pragma once
#include "Core/Kaguya.h"
#include "Geometry/Primitive.h"
#include "embree2/rtcore.h"

namespace Kaguya
{

class Scene
{
public:
    Scene();
    Scene(std::vector<std::shared_ptr<Primitive>> prims,
          std::vector<std::shared_ptr<Light>> lights);
    ~Scene();

    void commitScene();

    void addPrimitive(std::shared_ptr<Primitive> &prim)
    {
        mPrims.push_back(prim);
        buildGeometry(prim.get());
    }

    void addLight(std::shared_ptr<Light> &light)
    {
        mLights.push_back(light);
    }

    bool intersect(Ray &inRay,
                   DifferentialGeometry* dg,
                   Float* tHit,
                   Float* rayEpsilon) const;
private:
    void buildGeometry(const Primitive* prim);

    void buildUserGeomtry(const Primitive* prim);
    void buildPolygonalMesh(const PolyMesh* prim);
    void buildSubdivisionMesh(const SubdMesh* prim);
    void buildCurve(const Curve* prim);
    void buildInstance(const Primitive* prim);

private:
    RTCScene                                mSceneContext;

    //uint32_t                                mSceneID;
    std::vector<std::shared_ptr<Primitive>> mPrims;
    std::vector<std::shared_ptr<Light>>     mLights;
};

}
