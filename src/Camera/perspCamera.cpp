#include "perspCamera.h"

perspCamera::perspCamera()
	:lensRadius(0), focalDistance(INFINITY)

{
	/*nx = X_AXIS3D;
	ny = Y_AXIS3D;
	nz = Z_AXIS3D;*/
}
perspCamera::perspCamera(const Point3D& eyePos, const Vector3D& target,
	const Vector3D& upVec, Float lr, Float fd)
	: lensRadius(lr), focalDistance(fd)
{
	pos = eyePos;
	nz = Normalize(target - eyePos);
	nx = Normalize(Cross(nz, upVec));
	ny = Cross(nx, nz);
	Matrix4D projMat = setPerspective();
	CameraToWorld = lookAt(eyePos, target, upVec);
	CameraToScreen = Transform(projMat);
}

perspCamera::perspCamera(const Transform& cam2wo, const Transform& projection)
{
	CameraToWorld = cam2wo;
	CameraToScreen = projection;
}

perspCamera::~perspCamera()
{

}


Ray perspCamera::shootRay(Float imgX, Float imgY) const
{
	Vector2D fPos = film.getFilmPos(imgX, imgY);//point on film
	//Vector3D rayDir = nz * focLen + nx * fPos.x + ny * fPos.y;
	Vector3D rayDir = (CameraToWorld.getMat() * Vector4D(fPos.x, fPos.y, focLen, 0)).toVector3D();

	Ray ret(Point3D(CameraToWorld.getMat()[3]), Normalize(rayDir));
	if (lensRadius > 0)
	{
		//sample point on lens
		Float lensU, lensV;
		//sample lensU and lensV to (-1,1)
		lensU = unitRandom(20) * 2.0 - 1.0;
		lensV = unitRandom(20) * 2.0 - 1.0;
		lensU *= lensRadius;
		lensV *= lensRadius;//scale to focal radius

		//compute point on plane of focus
		Float ft = focalDistance / (ret.dir * nz);
		Point3D focusP = ret(ft);
		//update ray of lens
		ret.pos += pos + nx * lensU + ny * lensV;
		ret.dir = Normalize(focusP - ret.pos);

	}

	return ret;
}
Float perspCamera::generateRay(const cameraSampler &sample, Ray* ray) const
{
	Vector3D pCam = RasterToCamera(Vector3D(sample.imgX, sample.imgY, 0), 1.);
	*ray = Ray(Vector3D(), Normalize(pCam));
	// Depth of Field Operations;
	if (lensRadius > 0)
	{
		//sample point on lens
		Float lensU, lensV;
		//sample lensU and lensV to (-1,1)
		lensU = unitRandom(20) * 2.0 - 1.0;
		lensV = unitRandom(20) * 2.0 - 1.0;
		lensU *= lensRadius;
		lensV *= lensRadius;//scale to focal radius

		//compute point on plane of focus
		Float ft = focalDistance / ray->dir.z;
		Point3D focusP = (*ray)(ft);
		//update ray of lens
		ray->pos = Vector3D(lensU, lensV, 0);
		ray->dir = Normalize(focusP - ray->pos);
	}
	CameraToWorld(*ray, ray);
	return 1.0;
}
void perspCamera::renderImg(int x, int y, ColorRGBA& pixColor)
{
	film.setRGBA(x, y, pixColor);
}
void perspCamera::saveResult(const char* filename)
{
	film.writeFile(filename);
}


void perspCamera::setDoF(Float lr, Float fd)
{
	lensRadius = lr;
	focalDistance = fd;
}