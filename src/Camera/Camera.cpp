#include "Camera/Camera.h"
#include "Shading/Texture.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
baseCamera::baseCamera(const Vector3D& eyePos, const Vector3D& targetPos,
	const Vector3D& upVec)
	: CameraToWorld(lookAt(eyePos, target, upVec)), CameraToScreen(setPerspective())
	, target(targetPos)
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

Vector3D baseCamera::getTarget() const
{
	return target;
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
	//Matrix4D w2cam = CameraToWorld.getInvMat();
	Matrix4D cam2w = CameraToWorld.getMat();
	
	// Pw: world space position, Pc: Camera space position
	// Pw = c2w1 * Pc1 = c2w1 * T^-1 * Pc = c2w * Pc
	// c2w1 = T * c2w
	Matrix4D newLookAt = setTranslation(x_val, y_val, z_val) * cam2w;
	//cout << "befor: " << target << endl;

	Vector3D _nx(cam2w[0]), _ny(cam2w[1]);
	target += _nx * x_val + _ny * y_val;
	//cout << "after: " << target << endl;
	CameraToWorld.setMat(newLookAt);
}

void baseCamera::rotate(Float x_rot, Float y_rot, Float z_rot)
{
	//pitch, yaw, roll
	Matrix4D lookAtMat = CameraToWorld.getMat();

	Vector3D _pos(lookAtMat[3]);
	Vector3D vt = _pos - target;
	Float vt_len = vt.getLength();
	Float upCoef = lookAtMat[1][1] < 0 ? -1 : 1;
	
	Float phi = atan2(vt.x, vt.z) + DegreeToRadian(y_rot) * upCoef;
	Float old_theta = asin(vt.y / vt_len);
	Float theta = old_theta + DegreeToRadian(x_rot) * upCoef;
	
	if ((old_theta < M_HALFPI && theta > M_HALFPI) || (old_theta > -M_HALFPI && theta < -M_HALFPI))
	{
		upCoef *= -1;
	}
	Vector3D newVt(sin(phi) * cos(theta), sin(theta), cos(phi) * cos(theta));
	CameraToWorld = lookAt(target + newVt * vt_len, target, Vector3D(0, upCoef, 0));
}

void baseCamera::rotatePYR(Float pitchAngle, Float yawAngle, Float rollAngle)
{

	Matrix4D lookAtMat = CameraToWorld.getMat();
	Vector4D _nx(lookAtMat[0]), _ny(lookAtMat[1]), _nz(lookAtMat[2]);
	Vector4D _pos(lookAtMat[3]);
	Vector4D vt(_pos - Vector4D(target, 1));
	Float vt_len = vt.getLength();

	printf("Original VT: %lf\n", vt.getLength());
	// Rotate pitch
	Matrix4D pitchMat = setRotation(_nx.toVector3D(), pitchAngle);//setRotationX(pitchAngle);
	_ny = pitchMat * _ny;
	_nz = pitchMat * _nz;
	vt = pitchMat * vt;

	printf("\tafter pitch: %lf\n", vt.getLength());
	Matrix4D yawMat = setRotation(_ny.toVector3D(), yawAngle);//setRotationY(yawAngle);
	_nx = yawMat * _nx;
	_nz = yawMat * _nz;
	vt = yawMat * vt;

	printf("\tafter yaw: %lf\n", vt.getLength());
	_nx.normalize();
	_ny.normalize();
	_nz.normalize();
	vt.normalize();
	_pos = Vector4D(target, 1.0) + vt * vt_len;
	//Matrix4D newLookAt(_nx, _ny, _nz, _pos);
	CameraToWorld = lookAt(_pos.toVector3D(), target, _ny.toVector3D());
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
