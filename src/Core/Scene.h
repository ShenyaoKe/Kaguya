#pragma once
#include <embree2/rtcore.h>

#include "Core/Kaguya.h"
#include "Geometry/Geometry.h"

namespace Kaguya
{

class Scene
{
public:
	Scene(std::shared_ptr<Camera> camera,
		  std::vector<std::shared_ptr<Geometry>> prims
		  = std::vector<std::shared_ptr<Geometry>>(),
		  std::vector<std::shared_ptr<Light>> lights
		  = std::vector<std::shared_ptr<Light>>());
	~Scene();

	void commitScene();

	void addPrimitive(std::shared_ptr<Geometry> &prim)
	{
		mPrims.push_back(prim);
		buildGeometry(prim.get());
	}

	void addLight(std::shared_ptr<Light> &light)
	{
		mLights.push_back(light);
	}

	bool intersect(Ray &inRay,
				   Intersection* isec,
				   Float* tHit,
				   Float* rayEpsilon) const;

	RenderBufferTrait getRenderBuffer(uint32_t geomID) const;
	size_t getPrimitiveCount() const
	{
		return mPrims.size();
	}
	size_t getLightCount() const
	{
		return mLights.size();
	}

	std::shared_ptr<Camera> getCamera()
	{
		return mCamera;
	}
	const std::shared_ptr<Camera> getCamera() const
	{
		return mCamera;
	}

private:
	void buildGeometry(const Geometry* prim);

	void buildUserGeomtry(const Geometry* prim);
	void buildPolygonalMesh(const PolyMesh* prim);
	void buildSubdivisionMesh(const SubdMesh* prim);
	void buildCurve(const Curve* prim);
	void buildInstance(const Geometry* prim);

private:
	RTCScene                                mSceneContext;

	std::shared_ptr<Camera>                 mCamera;
	std::vector<std::shared_ptr<Geometry>>  mPrims;
	std::vector<std::shared_ptr<Light>>     mLights;
};

}
