#include "Camera/Camera.h"
#include "Shading/Texture.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
baseCamera::baseCamera(Vector3D& eyePos, Vector3D& target, Vector3D& upVec)
	: CameraToWorld(lookAt(eyePos, target, upVec)), CameraToScreen(setPerspective())
{
	/*pos = eyePos;
	nz = Normalize(target - eyePos);
	nx = Normalize(Cross(nz, nz));
	ny = Normalize(Cross(nx, nz));*/
}


baseCamera::baseCamera()
	: CameraToWorld(lookAt())
{
}

void baseCamera::setResolution(int resX, int resY)
{
	film.resize(resX, resY);
	buffer = renderBuffer(resX, resY);
}
void baseCamera::setSample(int aaSample)
{
	sample = aaSample;
}
void baseCamera::setFocLen(Float fl)
{
	focLen = fl;
}
void baseCamera::setFilmType(FILM_TYPE filmType)
{
	film.setFilmType(filmType);
}
/*
void baseCamera::lookAt(const Vector3D& targPos)
{
	nz = Normalize(targPos - pos);
	nx = nz.crossMul(ny);
	ny = nx.crossMul(nz);
}
void baseCamera::lookAt(const Vector3D& camPos, const Vector3D& targPos, const Vector3D& upDir)
{
	pos = camPos;
	nz = Normalize(targPos - camPos);
	nx = Cross(nz, upDir);
	ny = Cross(nx, nz);
}*/
void baseCamera::setBuffer(int x, int y, const bufferData tmpBuff)
{
	buffer.data[x][y] = tmpBuff;
}
bufferData baseCamera::getBufferData(int x, int y) const
{
	return buffer.data[x][y];
}

void baseCamera::updateCamToWorld(const Matrix4D &cam2wMat)
{
	CameraToWorld = Transform(cam2wMat);
}
void baseCamera::updateProjection(const Matrix4D &perspMat)
{
	CameraToScreen = Transform(perspMat);
}

void baseCamera::exportVBO(float *view, float *proj, float *raster) const
{
	if (view != nullptr)
	{
		CameraToWorld.mInv.exportVBO(view);
	}
	if (proj != nullptr)
	{
		CameraToScreen.m.exportVBO(proj);
	}
	if (raster != nullptr)
	{
		RasterToScreen.m.exportVBO(raster);
	}
}
//////////////////////////////////////////////////////////////////////////
perspCamera::perspCamera()
	:lensRadius(0), focalDistance(INFINITY)

{
	/*nx = X_AXIS3D;
	ny = Y_AXIS3D;
	nz = Z_AXIS3D;*/
}
perspCamera::perspCamera(const Point3D& eyePos, const Vector3D& target, const Vector3D& upVec,
	Float lr, Float fd) : lensRadius(lr), focalDistance(fd)
{
	/*pos = eyePos;
	nz = Normalize(target - eyePos);
	nx = Normalize(Cross(nz, upVec));
	ny = Cross(nx, nz);*/
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
	Vector3D rayDir = nz * focLen + nx * fPos.x + ny * fPos.y;

	Ray ret(pos, Normalize(rayDir));
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
void perspCamera::renderImg(int x, int y, ColorRGBA& pixColor)
{
	film.setRGBA(x, y, pixColor);
}
void perspCamera::saveResult(const char* filename)
{
	film.writeFile(filename);
}

void baseCamera::zoom(Float x_val, Float y_val, Float z_val)
{
	Matrix4D newLookAt = setTranslation(x_val, y_val, z_val) * CameraToWorld.getMat();
	CameraToWorld.setMat(newLookAt);
}

void baseCamera::rotate(Float x_rot /*= 0*/, Float y_rot /*= 0*/, Float z_rot /*= 0*/)
{
	Matrix4D newLookAt = setRotationX(x_rot) * setRotationY(y_rot) * setRotationZ(z_rot) * CameraToWorld.getInvMat();

	CameraToWorld.setInvMat(newLookAt);
}

void baseCamera::resizeViewport(Float aspr /*= 1.0*/)
{
	Matrix4D newProj= CameraToScreen.getMat();
	newProj[0][0] = -newProj[1][1] / aspr;
	CameraToScreen.setMat(newProj);
}

void perspCamera::setDoF(Float lr, Float fd)
{
	lensRadius = lr;
	focalDistance = fd;
}
//////////////////////////////////////////////////////////////////////////
/*
abstractCamera::abstractCamera() : perspCamera()
{
}

abstractCamera::abstractCamera(const Point3D& eyePos, const Vector3D& viewDir, const Vector3D& upVec,
	Texture *posImg, Texture *dirImg, Float tp, Float td, Float lr, Float fd)
	: perspCamera(eyePos, viewDir, upVec, lr, fd), tpos(tp), tdir(td), posTex(posImg), dirTex(dirImg)
{
}
abstractCamera::~abstractCamera()
{
	posTex = nullptr;
	dirTex = nullptr;
}

Ray abstractCamera::shootRay(Float imgX, Float imgY) const
{
	Vector2D fPos;// = film.getFilmPos(imgX, imgY);//point on film
	Vector3D rayDir;// = nz * focLen + nx * fPos.x + ny * fPos.y;

	Float newU = imgX / film.getWidth(), newV = imgY / film.getHeight();
	Point3D rayPoint = pos;

	if (posTex != nullptr)
	{
		ColorRGBA shiftPos = tpos * (posTex->getColor(Point3D(newU, newV, 0)) - ColorRGBA(0.5, 0.5, 0.5, 1));
		fPos = film.getFilmPos(shiftPos.r * film.getWidth() + imgX, shiftPos.g * film.getHeight() + imgY);
		rayDir = nx * fPos.x + ny * fPos.y + nz * focLen * (1 + shiftPos.b);
		rayPoint = pos + shiftPos.r * nx + shiftPos.g * ny + shiftPos.b * nz;
	}
	//Vector3D rayDir = nz * focLen + nx * fPos.x + ny * fPos.y;

	Ray ret(rayPoint, Normalize(rayDir));
	

	return ret;
}

void abstractCamera::setImage(Texture *posImg / *= nullptr* /, Texture *dirImg / *= nullptr* /)
{
	posTex = posImg;
	dirTex = dirImg;
}

void abstractCamera::setAbstraction(Float tp / *= 0* /, Float td / *= 0* /)
{
	tpos = tp;
	tdir = td;
}
*/

orthoCamera::orthoCamera()
{
	CameraToWorld = lookAt();
	CameraToScreen = Transform(setOrthographic());
}

orthoCamera::orthoCamera(const Transform& cam2wo, const Transform& projection)
{
	CameraToWorld = cam2wo;
	CameraToScreen = projection;
}

orthoCamera::~orthoCamera()
{

}

Ray orthoCamera::shootRay(Float imgX, Float imgY) const
{
	return Ray();
}
