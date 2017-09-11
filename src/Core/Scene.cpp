#include "Scene.h"
#include "Core/EmbreeUtils.h"
#include "Geometry/TriangleMesh.h"
#include "Geometry/QuadMesh.h"
#include "Geometry/SubdMesh.h"
#include "Geometry/Curve.h"

#include <embree2/rtcore_geometry.h>
#include <embree2/rtcore_ray.h>

namespace Kaguya
{

Scene::Scene()
	: mSceneContext(rtcDeviceNewScene(EmbreeUtils::getDevice(),
									  RTC_SCENE_STATIC,
									  RTC_INTERSECT1))
{
}

Scene::Scene(std::shared_ptr<Camera> camera,
			 std::vector<std::shared_ptr<RenderPrimitive>> prims,
			 std::vector<std::shared_ptr<Light>> lights)
	: mSceneContext(rtcDeviceNewScene(EmbreeUtils::getDevice(),
					RTC_SCENE_STATIC,
					RTC_INTERSECT1))
	, mCamera(camera)
	, mPrims(std::move(prims))
	, mLights(std::move(lights))
{
	for (auto& prim : mPrims)
	{
		buildGeometry(prim->getGeometry());
	}
}

Scene::~Scene()
{
}

void Scene::commitScene()
{
	rtcCommit(mSceneContext);
}

bool Scene::intersect(Ray &inRay,
					  Intersection* isec,
					  Float* tHit,
					  Float* rayEpsilon) const
{
#ifdef KAGUYA_DOUBLE_AS_FLOAT
	RTCRay ray = EmbreeUtils::safeConvert(inRay);
	rtcIntersect(mSceneContext, ray);
	if (ray.geomID != RTC_INVALID_GEOMETRY_ID)
	{
		return true;
	}
#else
	rtcIntersect(mSceneContext, *(static_cast<RTCRay*>((void*)&inRay)));
	if (inRay.geomID != RTC_INVALID_GEOMETRY_ID)
	{
		isec->shape = mPrims[inRay.geomID]->getGeometry();
		isec->shape->postIntersect(inRay, isec);
		return true;
	}
#endif
	return false;
}

RenderBufferTrait Scene::getRenderBuffer(uint32_t geomID) const
{
	RenderBufferTrait ret;
	mPrims.at(geomID)->getGeometry()->getRenderBuffer(&ret);
	return ret;
}

void Scene::buildGeometry(const Geometry* prim)
{
	if (prim == nullptr)
	{
		return;
	}
	switch (prim->primitiveType())
	{
	case GeometryType::PARAMATRIC_SURFACE:
	{
		buildUserGeomtry(prim);
		break;
	}
	case GeometryType::POLYGONAL_MESH:
	{
		buildPolygonalMesh(static_cast<const PolyMesh*>(prim));
		break;
	}
	case GeometryType::SUBDIVISION_MESH:
	{
		buildSubdivisionMesh(static_cast<const SubdMesh*>(prim));
		break;
	}
	case GeometryType::CURVE:
	{
		buildCurve(static_cast<const Curve*>(prim));
		break;
	}
	case GeometryType::INSTANCE:
	{
		buildInstance(prim);
		break;
	}
	default:
	{
		buildUserGeomtry(prim);
		break;
	}
	}
}

void Scene::buildUserGeomtry(const Geometry* prim)
{
	// TODO
}

void Scene::buildPolygonalMesh(const PolyMesh* prim)
{
	TessBuffer buffer;
	prim->getTessellated(buffer);
	uint32_t geomID = (prim->polyMeshType() == PolyMeshType::TRIANGLE
					   ? &rtcNewTriangleMesh2
					   : &rtcNewQuadMesh2)(mSceneContext,
										  RTC_GEOMETRY_STATIC,
										  buffer.nPrimtives, //!< number of quads
										  buffer.nVertices,  //!< number of vertices
										  buffer.nTimeStep,  //!< number of motion blur time steps
										  buffer.nGeomId);

	rtcSetBuffer(mSceneContext, geomID, RTC_VERTEX_BUFFER0,
				 buffer.vertTraits[0].data,
				 buffer.vertTraits[0].byteOffset,
				 buffer.vertTraits[0].byteStride);
	if (buffer.nTimeStep > 1)
	{
		rtcSetBuffer(mSceneContext, geomID, RTC_VERTEX_BUFFER1,
					 buffer.vertTraits[1].data,
					 buffer.vertTraits[1].byteOffset,
					 buffer.vertTraits[1].byteStride);
	}
	rtcSetBuffer(mSceneContext, geomID, RTC_INDEX_BUFFER,
				 buffer.indexTrait.data,
				 buffer.indexTrait.byteOffset,
				 buffer.indexTrait.byteStride);
}

void Scene::buildSubdivisionMesh(const SubdMesh* prim)
{
	// TODO
}

void Scene::buildCurve(const Curve* prim)
{
	// TODO
}

void Scene::buildInstance(const Geometry* prim)
{
	// TODO
}

}
