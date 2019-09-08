#include "AklemanCamera.h"
#include "Shading/Texture.h"

namespace Kaguya {

AklemanCamera::AklemanCamera()
	: PerspectiveCamera()
{
}

AklemanCamera::AklemanCamera(const Point3f    &eyePos,
							 const Vector3f   &viewDir,
							 const Vector3f   &upVec,
							 Texture*    posImg,
							 Texture*    dirImg,
							 Float       tp,
							 Float       td,
							 Float       lr,
							 Float       fd)
    : PerspectiveCamera(eyePos, eyePos + viewDir, upVec, lr, fd)
    , posTex(posImg), dirTex(dirImg)
    , tpos(tp), tdir(td)
{
}

AklemanCamera::~AklemanCamera()
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
	Point3f rayPoint = pos;

	if (posTex != nullptr)
	{
		ColorRGBA shiftPos = tpos * (posTex->getColor(Point3f(newU, newV, 0)) - ColorRGBA(0.5, 0.5, 0.5, 1));
		fPos = film.getFilmPos(shiftPos.r * film.getWidth() + imgX, shiftPos.g * film.getHeight() + imgY);
		rayDir = nx * fPos.x + ny * fPos.y + nz * focLen * (1 + shiftPos.b);
		rayPoint = pos + shiftPos.r * nx + shiftPos.g * ny + shiftPos.b * nz;
	}
	//Vector3D rayDir = nz * focLen + nx * fPos.x + ny * fPos.y;

	Ray ret(rayPoint, Normalize(rayDir));

	return ret;
}
*/
void AklemanCamera::setImage(Texture* posImg, Texture* dirImg)
{
	posTex = posImg;
	dirTex = dirImg;
}

void AklemanCamera::setAbstraction(Float tp, Float td)
{
	tpos = tp;
	tdir = td;
}

}