#include "Scene.h"
#include "Core/EmbreeUtils.h"
#include "embree2/rtcore_geometry.h"

namespace Kaguya
{

Scene::Scene(std::vector<std::shared_ptr<Primitive>> prims,
             std::vector<std::shared_ptr<Light>> lights)
    : mSceneContext(nullptr)
    , mPrims(std::move(prims))
    , mLights(std::move(lights))
{

    mSceneContext = rtcDeviceNewScene(EmbreeUtils::getDevice(),
                                      RTC_SCENE_STATIC,
                                      RTC_INTERSECT1);

    for (int i = 0; i < mPrims.size(); i++)
    {
        buildGeometry(mPrims[i].get());
    }
}

Scene::~Scene()
{
}

void Scene::buildGeometry(const Primitive* prim)
{
    switch (true)
    {
    default:
        break;
    }
}

}
