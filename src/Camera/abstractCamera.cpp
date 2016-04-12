#include "abstractCamera.h"
#include "Shading/Texture.h"


abstractCamera::abstractCamera()
	: perspCamera()
{
}

abstractCamera::abstractCamera(const Vector3D &eyePos, const Vector3D &viewDir, const Vector3D &upVec,
	Texture *posImg, Texture *dirImg, Float tp, Float td, Float lr, Float fd)
	: perspCamera(eyePos, viewDir, upVec, lr, fd), tpos(tp), tdir(td), posTex(posImg), dirTex(dirImg)
{
}
abstractCamera::~abstractCamera()
{
	posTex = nullptr;
	dirTex = nullptr;
}
/*
Ray abstractCamera::generateRay(Float imgX, Float imgY) const
{
	Vector2D fPos;// = film.getFilmPos(imgX, imgY);//point on film
	Vector3D rayDir;// = nz * focLen + nx * fPos.x + ny * fPos.y;

	Float newU = imgX / film.height, newV = imgY / film.height;
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
*/
void abstractCamera::setImage(Texture *posImg, Texture *dirImg)
{
	posTex = posImg;
	dirTex = dirImg;
}

void abstractCamera::setAbstraction(Float tp, Float td)
{
	tpos = tp;
	tdir = td;
}