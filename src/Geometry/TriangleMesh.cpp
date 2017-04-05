#include "Geometry/TriangleMesh.h"
#include "Tracer/Ray.h"
#include "Geometry/DifferentialGeometry.h"
#include "Shading/Shader.h"
#include "Shading/TextureMapping.h"
#include "Shading/Texture.h"

namespace Kaguya
{

TriangleMesh::TriangleMesh(std::vector<Point3f>             &vertexBuffer,
                           std::vector<uint32_t>            &indexBuffer,
                           std::vector<uint32_t>            &faceSizeBuffer,
                           size_t                            totalPrimCount,
                           std::shared_ptr<TextureAttribute> texAttri,
                           std::shared_ptr<NormalAttribute>  normAttri,
                           bool                              isTessellated)
    : PolyMesh(vertexBuffer, indexBuffer,
               vertexBuffer.size(), totalPrimCount,
               texAttri, normAttri)
{
    if (!isTessellated)
    {
        tessellate(mIndexBuffer, faceSizeBuffer, totalPrimCount);
        if (mTextureAttribute->isFaceVarying())
        {
            tessellate(mTextureAttribute->mIndexBuffer,
                       faceSizeBuffer,
                       totalPrimCount);
        }
        if (mNormalAttibute->isFaceVarying())
        {
            tessellate(mNormalAttibute->mIndexBuffer,
                       faceSizeBuffer,
                       totalPrimCount);
        }
    }
}

TriangleMesh::~TriangleMesh()
{
}

void TriangleMesh::bounding()
{
    for (auto &v : mVertexBuffer)
    {
        mObjBound.Union(v);
    }
}

void TriangleMesh::printInfo(const std::string &msg) const
{
   /* if (!msg.empty())
    {
        std::cout << msg << std::endl;
    }
    for (int i = 0; i < mVertexBuffer.size(); i++)
    {
        std::cout << "Vertex:\t" << mVertexBuffer[i] << std::endl;
    }
    for (int i = 0; i < uvs.size(); i++)
    {
        std::cout << "UV:\t" << uvs[i] << std::endl;
    }
    for (int i = 0; i < norms.size(); i++)
    {
        std::cout << "Normal:\t" << norms[i] << std::endl;
    }
    for (int i = 0; i < fids.size(); i++)
    {
        std::cout << "Faces:\t";
        fids[i].printInfo();
    }*/
}

bool TriangleMesh::intersect(const Ray &inRay, DifferentialGeometry* dg,
                             Float* tHit, Float* rayEpsilon) const
{
    return false;
}

void TriangleMesh::postIntersect(const Ray &inRay, DifferentialGeometry* dg) const
{
    // TODO: Implement post-intersection method
    uint32_t primID = inRay.primID;
    uint32_t id1 = mIndexBuffer[primID * sTriFaceSize];
    uint32_t id2 = mIndexBuffer[primID * sTriFaceSize + 1];
    uint32_t id3 = mIndexBuffer[primID * sTriFaceSize + 2];

    dg->Ng = inRay.Ng;
    dg->UV = { inRay.u, inRay.v };
    Float s = dg->UV.x;
    Float t = dg->UV.y;
    Float w = 1.0 - s - t;
    dg->P = mVertexBuffer[id1] * w
          + mVertexBuffer[id2] * s
          + mVertexBuffer[id3] * t;
}

void TriangleMesh::getTessellated(TessBuffer &trait) const
{
    // TODO: Implementation check required
    // Setup time step
    size_t timestep = 1;
    trait.nTimeStep = timestep;
    
    // Setup first vertex buffer
    trait.nVertices = mVertexCount;
    trait.vertTraits.resize(timestep);
    trait.vertTraits[0].byteOffset = 0;
    trait.vertTraits[0].byteStride = sizeof(Point3f);
    trait.vertTraits[0].data = (void*)(mVertexBuffer.data());

    // Setup buffers for Motion Blur
    for (size_t i = 1; i < timestep; i++)
    {
        trait.vertTraits[i].byteOffset = 0;
        trait.vertTraits[i].byteStride = sizeof(Point3f);
    	trait.vertTraits[i].data = (void*)(mVertexBuffer.data());
    }

    // Setup index buffer
    trait.nPrimtives = mFaceCount;
    trait.indexTrait.byteOffset = 0;
    trait.indexTrait.byteStride = sizeof(uint32_t) * sTriFaceSize;
    trait.indexTrait.data = (void*)(mIndexBuffer.data());
}

void TriangleMesh::tessellate(std::vector<uint32_t> &indexBuffer,
                              std::vector<uint32_t> &faceSizeBuffer,
                              size_t                 tessellatedCount)
{
    size_t nCurrentPosition = indexBuffer.size();
    size_t nLastPosition = tessellatedCount * sTriFaceSize;
    indexBuffer.resize(nLastPosition);

    for (int i = faceSizeBuffer.size() - 1; i >= 0; i--)
    {
        nCurrentPosition -= faceSizeBuffer[i];

        for (int j = faceSizeBuffer[i] - 2; j > 0; j--)
        {
            indexBuffer[--nLastPosition] = indexBuffer[nCurrentPosition + j + 1];
            indexBuffer[--nLastPosition] = indexBuffer[nCurrentPosition + j];
            indexBuffer[--nLastPosition] = indexBuffer[nCurrentPosition];
        }
    }
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

bool TriangleUtils::intersect(const Point3f &p0,
                              const Point3f &p1,
                              const Point3f &p2,
                              Ray &inRay,
                              Float* tHit, Float* rayEpsilon)
{
    // Moller¨CTrumbore Intersection Algorithm
    Vector3f v1 = p1 - p0;
    Vector3f v2 = p2 - p0;
    Vector3f areaVec = cross(v1, v2);
    Vector3f normal = normalize(areaVec);

    //ray triangle DifferentialGeometry length
    Float rayt = dot(normal, (p0 - inRay.o)) / dot(normal, inRay.d);
    if (rayt < inRay.tMin || rayt > inRay.tMax) return false;

    //inRay.tmin = rayT;
    Point3f ph = inRay(rayt);
    Vector3f A2 = cross(p0 - ph, p1 - ph);
    Vector3f A1 = cross(p2 - ph, p0 - ph);

    int maxIndex = std::abs(areaVec[0]) > std::abs(areaVec[1]) ? 0 : 1;
    maxIndex = std::abs(areaVec[maxIndex]) > std::abs(areaVec[2]) ? maxIndex : 2;
    // Barycentric coordinate
    Float u = A1[maxIndex] / areaVec[maxIndex];
    Float v = A2[maxIndex] / areaVec[maxIndex];
    Float w = 1.0 - u - v;
    if (!inUnitRange(u) || !inUnitRange(v) || !inUnitRange(w))
    {
        return false;
    }

    inRay.u = u;
    inRay.v = v;
    *tHit = rayt;
    *rayEpsilon = reCE * *tHit;

    return true;
}

bool TriangleUtils::intersectWatertight(const Point3f &p0,
                                        const Point3f &p1,
                                        const Point3f &p2,
                                        Ray &inRay,
                                        Float* tHit, Float* rayEpsilon)
{
    Vector3f vro = Vector3f(inRay.o);
    Point3f v0 = p0 - vro;
    Point3f v1 = p1 - vro;
    Point3f v2 = p2 - vro;

    // Permute Ray
    int kz = maxDimension(abs(inRay.d));
    int kx = kz + 1; if (kx == 3) kx = 0;
    int ky = kx + 1; if (ky == 3) ky = 0;
    Vector3f pmDir = permute(inRay.d, kx, ky, kz);

    // Permute Points
    v0 = permute(v0, kx, ky, kz);
    v1 = permute(v1, kx, ky, kz);
    v2 = permute(v2, kx, ky, kz);

    // Shear Ray
    float sz = 1.0f / pmDir.z;
    float sx = -pmDir.x * sz;
    float sy = -pmDir.y * sz;

    // Shear Points
    v0.x += sx * v0.z;
    v0.y += sy * v0.z;
    v1.x += sx * v1.z;
    v1.y += sy * v1.z;
    v2.x += sx * v2.z;
    v2.y += sy * v2.z;

    // area = (p_i+1 - p_i) x (p - p_i), where p = (0, 0)
    //      = p_i x p_i+1 = x_i * y_i+1 - x_i+1 * y_i
    Float e0 = v1.x * v2.y - v2.x * v1.y;
    Float e1 = v2.x * v0.y - v0.x * v2.y;
    Float e2 = v0.x * v1.y - v1.x * v0.y;
    if (e0 < 0 || e1 < 0 || e2 < 0) return false;

    v0.z *= sz;
    v1.z *= sz;
    v2.z *= sz;
    Float det = e0 + e1 + e2;
    Float invDet = 1.0f / det;
    // TODO: finish intersection
    // Barycentric Coordinates
    Float u = e0 * invDet;
    Float v = e1 * invDet;

    return true;
}

void TriangleUtils::postIntersect(const Point3f &p0,
                                  const Point3f &p1,
                                  const Point3f &p2,
                                  const Ray &inRay,
                                  DifferentialGeometry* dg)
{
    Vector3f v1 = p1 - p0;
    Vector3f v2 = p2 - p0;

    // Compute dpdu, dpdv
    // 
#if 0
    Vector3f dpdu, dpdv;
    Float du1 = uv[1]->x - uv[0]->x;
    Float dv1 = uv[1]->y - uv[0]->y;
    Float du2 = uv[2]->x - uv[0]->x;
    Float dv2 = uv[2]->y - uv[0]->y;
    Float detUV = du1 * dv2 - dv1 * du2;
    if (detUV == 0.0)
    {
        coordinateSystem(Vector3f(dg->Ng), &dg->dPdu, &dg->dPdv);
    }
    else
    {
        Float invDetUV = 1. / detUV;
        dg->dPdu = (v1 * dv2 - v2 * dv1) * invDetUV;
        dg->dPdv = (v2 * du1 - v1 * du2) * invDetUV;
    }

    // Interpolate Texture Coordinates
    Float s = dg->UV.x;
    Float t = dg->UV.y;
    Float w = 1.0 - s - t;
    dg->shading.ST = *uv[0] * w + *uv[1] * s + *uv[2] * t;
#endif
}

}
