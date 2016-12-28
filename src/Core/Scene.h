#pragma once
#include "Core/Kaguya.h"
#include "Geometry/Primitive.h"
#include "embree2/rtcore.h"

namespace Kaguya
{

class Scene
{
public:
    Scene(std::vector<std::shared_ptr<Primitive>> prims,
          std::vector<std::shared_ptr<Light>> lights);
    ~Scene();
private:
    void buildGeometry(const Primitive* prim);

private:
    RTCScene                                mSceneContext;

    uint32_t                                mSceneID;
    std::vector<std::shared_ptr<Primitive>> mPrims;
    std::vector<std::shared_ptr<Light>>     mLights;
};

}
