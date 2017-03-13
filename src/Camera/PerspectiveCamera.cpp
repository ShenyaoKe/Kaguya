#include "PerspectiveCamera.h"

namespace Kaguya
{

PerspectiveCamera::PerspectiveCamera(const Point3f &eye,
                                     const Point3f &targ,
                                     const Vector3f &up,
                                     Float aspectRatio,
                                     Float lensRadius,
                                     Float focalDist,
                                     const Film &film)
    : ProjectiveCamera(eye, targ, up, aspectRatio, lensRadius, focalDist, film)
{
    updateTransform();
}

PerspectiveCamera::PerspectiveCamera(const Transform &cam2wo, const Transform &projection)
{
    CameraToWorld = cam2wo;
    CameraToScreen = projection;
}

PerspectiveCamera::~PerspectiveCamera()
{

}

void PerspectiveCamera::updateCamToScreen()
{
    Float horisize, vertsize;

    if (mViewportRatio - mFilm.ApectureRatio() < 0)
    {
        horisize = mFilm.horiApect;
        vertsize = horisize / mViewportRatio;
    }
    else
    {
        vertsize = mFilm.vertApect;
        horisize = vertsize * mViewportRatio;
    }

    CameraToScreen.setMat(Matrix4x4::perspectiveFromFilm(
        vertsize, horisize, mFocalLength, mNearPlane, mFarPlane));
}

Float PerspectiveCamera::generateRay(const cameraSampler &sample,
                                     Ray* ray) const
{
    Point3f pCam = RasterToCamera(Point3f(sample.imgX, sample.imgY, 0));
    *ray = Ray(Point3f(), normalize(Vector3f(pCam)));
    // Depth of Field Operations;
    if (mLensRadius > 0.)
    {
        //sample point on lens
        Float lensU, lensV;
        //sample lensU and lensV to (-1,1)
        lensU = unitRandom(20) * 2.0 - 1.0;
        lensV = unitRandom(20) * 2.0 - 1.0;
        lensU *= mLensRadius;
        lensV *= mLensRadius;//scale to focal radius

        //compute point on plane of focus
        Float ft = mFocalDistance / ray->d.z;
        Point3f focusP = (*ray)(ft);
        //update ray of lens
        ray->o = Point3f(lensU, lensV, 0);
        ray->d = normalize(focusP - ray->o);
    }
    CameraToWorld(*ray, ray);
    return 1.0;
}

void PerspectiveCamera::renderImg(int x, int y, ColorRGBA &pixColor)
{
    //film.setRGBA(x, y, pixColor);
}

void PerspectiveCamera::saveResult(const char* filename)
{
    //film.writeFile(filename);
}

void PerspectiveCamera::resizeViewport(Float aspr)
{
    mViewportRatio = aspr;
    updateCamToScreen();
    updateRasterToScreen();
}

void PerspectiveCamera::setDoF(Float lr, Float fd)
{
    mLensRadius = lr;
    mFocalDistance = fd;
}

}
