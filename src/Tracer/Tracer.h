#ifndef __TRACER__
#define __TRACER__
//#include "Core/rtdef.h"
#include "Image/ColorData.h"
#include "Accel/KdTreeAccel.h"

struct render_setting
{
	const int width = 960, height = 720;
	unsigned char *pixmap;
	const int aaSample = 1;
	Float* kernal;
	const bool IS_RENDER_SHADOW = false;
	enum RENDER_VIEW_TYPE
	{
		RENDER_BEAUTY = 0,
		RENDER_DIFFUSE = 1,

		RENDER_SPECULAR = 2,
		RENDER_ZDEPTH = 3,
		RENDER_NORMAL = 4,
		RENDER_OBJECT_ID = 5
	};
	const RENDER_VIEW_TYPE viewType = RENDER_BEAUTY;
	Float eta = 1.3;//IOR
	const int refraction_depth = 3;
	const int reflection_depth = 1;
	const int time_duaration = 24;
	const Float invDuaration = 2 * M_PI / static_cast<Float>(time_duaration);
	int frame_no = 0;
	const Float shutter_speed = 0.4;
	const Float glossy = 0.2;
	const Float translucency = 0;//0.3;
	const Float lensRadius = 0.08;
	Float focalDist = 5.0;
};
/*
ColorRGBA render_refraction(const KdTreeAccel *sceneTree, Ray &inRay, DifferentialGeometry* queryPoint, Float &tHit, Float &rayEp)
{
	ColorRGBA retColor;
	do
	{
		/ *cout << inRay.dp << "\t";
		inRay.dir.printInfo();* /
		inRay.dp++;

		Float tr_dir, tr_norm, tr_theta;
		//eta = queryPoint.object->getAmbient(queryPoint).r * 2.0;
		tr_theta = -inRay.dir * queryPoint->normal;
		Float eta2 = tr_theta > 0 ? eta : 1.0 / eta;
		tr_dir = 1.0 / eta2;
		tr_norm = sqr(tr_theta) + sqr(eta2) - 1;
		if (tr_norm < 0)
		{
			break;
		}
		tr_theta = abs(tr_theta);
		tr_norm = (tr_theta - sqrt(tr_norm)) / eta2;

		inRay.dir = inRay.dir * tr_dir + queryPoint->normal * tr_norm;
		if (translucency != 1.0)
		{
			inRay.dir += Vector3D(unitRandom(20) - 0.5, unitRandom(20) - 0.5, unitRandom(20) - 0.5) * sqr(translucency);
		}
		inRay.dir.normalize();
		inRay.pos = queryPoint->pos + rayEp * inRay.dir;

		Float tHit = INFINITY;
		if (!sceneTree->hit(inRay, queryPoint, &tHit, &rayEp))
		{
			break;
		}

	} while (inRay.dp < refraction_depth);

	retColor = envm->getColor(inRay.dir);
	return retColor;
}
ColorRGBA render_refraction(const KdTreeAccel *sceneTree, Ray &inRay, DifferentialGeometry* queryPoint, Float &tHit, Float &rayEp)
{
	ColorRGBA retColor;
	do
	{
		/ *cout << inRay.dp << "\t";
		inRay.dir.printInfo();* /
		inRay.dp++;

		Float tr_dir, tr_norm, tr_theta;
		//eta = queryPoint.object->getAmbient(queryPoint).r * 2.0;
		tr_theta = -inRay.dir * queryPoint->normal;
		Float eta2 = tr_theta > 0 ? eta : 1.0 / eta;
		tr_dir = 1.0 / eta2;
		tr_norm = sqr(tr_theta) + sqr(eta2) - 1;
		if (tr_norm < 0)
		{
			break;
		}
		tr_theta = abs(tr_theta);
		tr_norm = (tr_theta - sqrt(tr_norm)) / eta2;

		inRay.dir = inRay.dir * tr_dir + queryPoint->normal * tr_norm;
		if (translucency != 1.0)
		{
			inRay.dir += Vector3D(unitRandom(20) - 0.5, unitRandom(20) - 0.5, unitRandom(20) - 0.5) * sqr(translucency);
		}
		inRay.dir.normalize();
		inRay.pos = queryPoint->pos + rayEp * inRay.dir;

		Float tHit = INFINITY;
		if (!sceneTree->hit(inRay, queryPoint, &tHit, &rayEp))
		{
			break;
		}

	} while (inRay.dp < refraction_depth);

	retColor = envm->getColor(inRay.dir);
	return retColor;
}*/
#endif // __TRACER__
