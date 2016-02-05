#include "perspCamera.h"

perspCamera::perspCamera(const Vector3D& eye,
	const Vector3D& targ, const Vector3D& up,
	Float asp, Float lr, Float fd, const Film &fm)
	: Camera(eye, targ, up, asp, lr, fd, fm)
{
	/*Matrix4D projMat = Matrix4D::Perspective();
	CameraToWorld.setMat(Matrix4D::LookAt(eye, target, up));
	
	CameraToScreen = Transform(projMat);*/
	updateMatrices();
}

perspCamera::perspCamera(const Transform& cam2wo, const Transform& projection)
{
	CameraToWorld = cam2wo;
	CameraToScreen = projection;
}

perspCamera::~perspCamera()
{

}

void perspCamera::updateCamToScreen()
{
	Float horisize, vertsize;

	if (viewportRatio - film.ApectureRatio() < 0)
	{
		horisize = film.HorizonalApect() * 0.5;
		vertsize = horisize / viewportRatio;
	}
	else
	{
		vertsize = film.VerticalApect() / 0.5;
		horisize = vertsize * viewportRatio;
	}
	CameraToScreen.setMat(Matrix4D::PerspectiveFromFilm(
		vertsize, horisize, focLen, nearPlane, farPlane));
}

Ray perspCamera::generateRay(Float imgX, Float imgY) const
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
		/*Float ft = focalDistance / (ret.dir * nz);
		Point3D focusP = ret(ft);
		//update ray of lens
		ret.pos += pos + nx * lensU + ny * lensV;
		ret.dir = Normalize(focusP - ret.pos);*/

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


void perspCamera::resizeViewport(Float aspr /*= 1.0*/)
{
	viewportRatio = aspr;
	updateCamToScreen();
	updateRasterToScreen();
}

void perspCamera::setDoF(Float lr, Float fd)
{
	lensRadius = lr;
	focalDistance = fd;
}