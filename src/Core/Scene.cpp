#include "Scene.h"
#include "Core/EmbreeUtils.h"
#include "Geometry/TriangleMesh.h"
#include "Geometry/QuadMesh.h"
#include "Geometry/SubdMesh.h"
#include "Geometry/Curve.h"

#include <embree3/rtcore_geometry.h>
#include <embree3/rtcore_ray.h>

namespace Kaguya
{

Scene::Scene()
	: mSceneContext(rtcNewScene(EmbreeUtils::getDevice()))
{
}

Scene::Scene(std::shared_ptr<Camera> camera,
			 std::vector<std::shared_ptr<RenderPrimitive>> prims,
			 std::vector<std::shared_ptr<Light>> lights)
	: mSceneContext(rtcNewScene(EmbreeUtils::getDevice()))
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
	rtcReleaseScene(mSceneContext);
}

void Scene::commitScene()
{
	rtcCommitScene(mSceneContext);
}

bool Scene::intersect(Ray &inRay,
					  Intersection* isec,
					  Float* /*tHit*/,
					  Float* /*rayEpsilon*/) const
{
#ifdef KAGUYA_DOUBLE_AS_FLOAT
	RTCRay ray = EmbreeUtils::safeConvert(inRay);
	{
		RTCIntersectContext context;
		rtcInitIntersectContext(&context);
		rtcIntersect1(mSceneContext, &context, &ray);
		ray.hit.Ng_x = -ray.hit.Ng_x; // EMBREE_FIXME: only correct for triangles,quads, and subdivision surfaces
		ray.hit.Ng_y = -ray.hit.Ng_y;
		ray.hit.Ng_z = -ray.hit.Ng_z;
	}
	if (ray.geomID != RTC_INVALID_GEOMETRY_ID)
	{
		return true;
	}
#else
	{
		RTCIntersectContext context;
		rtcInitIntersectContext(&context);
		rtcIntersect1(mSceneContext, &context,
					  static_cast<RTCRayHit*>((void*)&inRay));
	}
	if (inRay.geomID != RTC_INVALID_GEOMETRY_ID)
	{
		isec->mShape = mPrims[inRay.primID]->getGeometry();
		isec->mShape->postIntersect(inRay, isec);
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

void Scene::buildUserGeomtry(const Geometry* /*prim*/)
{
	// TODO
}

void Scene::buildPolygonalMesh(const PolyMesh* prim)
{
	TessBuffer buffer;
	prim->getTessellated(buffer);
	RTCGeometryType geomType;
	RTCFormat indexType;
	switch (prim->polyMeshType())
	{
	case PolyMeshType::TRIANGLE:
		geomType = RTC_GEOMETRY_TYPE_TRIANGLE;
		indexType = RTC_FORMAT_UINT3;
		break;
	case PolyMeshType::QUAD:
		geomType = RTC_GEOMETRY_TYPE_QUAD;
		indexType = RTC_FORMAT_UINT4;
		break;
	default:
		break;
	}

	RTCGeometry embreeMesh = rtcNewGeometry(EmbreeUtils::getDevice(), geomType);

	rtcSetSharedGeometryBuffer(embreeMesh,
							   RTC_BUFFER_TYPE_VERTEX,
							   0,
							   RTC_FORMAT_FLOAT3,
							   buffer.vertTraits[0].data,
							   buffer.vertTraits[0].byteOffset,
							   buffer.vertTraits[0].byteStride,
							   buffer.nVertices);
	if (buffer.nTimeStep > 1)
	{
		rtcSetSharedGeometryBuffer(embreeMesh,
								   RTC_BUFFER_TYPE_VERTEX,
								   1,
								   RTC_FORMAT_FLOAT3,
								   buffer.vertTraits[1].data,
								   buffer.vertTraits[1].byteOffset,
								   buffer.vertTraits[1].byteStride,
								   buffer.nVertices);
	}
	rtcSetSharedGeometryBuffer(embreeMesh,
							   RTC_BUFFER_TYPE_INDEX,
							   0,
							   indexType,
							   buffer.indexTrait.data,
							   buffer.indexTrait.byteOffset,
							   buffer.indexTrait.byteStride,
							   buffer.nPrimtives);

	rtcCommitGeometry(embreeMesh);
	unsigned int geomID = rtcAttachGeometry(mSceneContext, embreeMesh);
	rtcReleaseGeometry(embreeMesh);
}

void Scene::buildSubdivisionMesh(const SubdMesh* /*prim*/)
{
	// TODO
}

void Scene::buildCurve(const Curve* /*prim*/)
{
	// TODO
}

void Scene::buildInstance(const Geometry* /*prim*/)
{
	// TODO
}

}
