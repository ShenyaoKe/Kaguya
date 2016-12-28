#include "PerspectiveCamera.h"

namespace Kaguya
{

PerspectiveCamera::PerspectiveCamera(const Point3f &eye,
                                     const Point3f &targ,
                                     const Vector3f &up,
                                     Float asp,
                                     Float lr,
                                     Float fd,
                                     const Film &fm)
    : ProjectiveCamera(eye, targ, up, asp, lr, fd, fm)
{
    updateMatrices();
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

    if (viewportRatio - film.ApectureRatio() < 0)
    {
        horisize = film.horiApect;
        vertsize = horisize / viewportRatio;
    }
    else
    {
        vertsize = film.vertApect;
        horisize = vertsize * viewportRatio;
    }

    CameraToScreen.setMat(Matrix4x4::perspectiveFromFilm(
        vertsize, horisize, focLen, nearPlane, farPlane));
}

Float PerspectiveCamera::generateRay(const cameraSampler &sample,
                                     Ray* ray) const
{
    Point3f pCam = RasterToCamera(Point3f(sample.imgX, sample.imgY, 0));
    *ray = Ray(Point3f(), normalize(Vector3f(pCam)));
    // Depth of Field Operations;
    if (lensRadius > 0.)
    {
        //sample point on lens
        Float lensU, lensV;
        //sample lensU and lensV to (-1,1)
        lensU = unitRandom(20) * 2.0 - 1.0;
        lensV = unitRandom(20) * 2.0 - 1.0;
        lensU *= lensRadius;
        lensV *= lensRadius;//scale to focal radius

        //compute point on plane of focus
        Float ft = focalDistance / ray->d.z;
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
    viewportRatio = aspr;
    updateCamToScreen();
    updateRasterToScreen();
}

void PerspectiveCamera::setDoF(Float lr, Float fd)
{
    lensRadius = lr;
    focalDistance = fd;
}

}
