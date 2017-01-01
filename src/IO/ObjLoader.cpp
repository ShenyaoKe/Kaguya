#include "ObjLoader.h"
#include "Geometry/Primitive.h"
#include "Geometry/PolyMesh.h"
#include "Core/Utils.h"

namespace Kaguya
{

ObjLoader::ObjLoader(const std::string &filename)
    : fp(std::fopen(filename.c_str(), "r"))
    , mFilename(filename)
{
    if (fp != nullptr)
    {
        parse(fp);
    }
    if (!faceIndexBuffer.empty())
    {
        mPrims.emplace_back(finalizeMesh());
    }
}

ObjLoader::~ObjLoader()
{

}

ObjLoader::index_t ObjLoader::facetype(const char* str, int32_t* val)
{
    int argv = sscanf(str, "%d/%d/%d", val, val + 1, val + 2);
    switch (argv)
    {
    case 3:// V/T/N
        return VTN;//111
    case 2:// V/T
        return VT;//011
    case 1:
        argv = sscanf(str, "%d//%d", val, val + 2);
        if (argv == 2)// V//N
        {
            return VN;//101
        }
    default:// V
        return V;;//001
    }
}

void ObjLoader::parse(std::FILE* fp)
{
    int err;
    char buff[256] = {};
    char lineHeader[3] = {};
    Float val[3] = {};

    while (true)
    {
        lineHeader[0] = lineHeader[1] = 0;
        err = fscanf(fp, "%2s", &lineHeader);
        if (err == EOF)
        {
            break;
        }

        bool isMeshBoundary = Utils::startsWith(lineHeader, "g")
            || Utils::startsWith(lineHeader, "o")
            || Utils::startsWith(lineHeader, "s")
            || Utils::startsWith(lineHeader, "usemtl");
        if (isMeshBoundary && !faceIndexBuffer.empty())
        {
            mPrims.emplace_back(finalizeMesh());
            clearMeshBuffers();
            swapBuffers();
            //get new filename
            fscanf(fp, "%s\n", buff);
            mFilename = std::string(buff);
        }

        // Vertex
        if (Utils::startsWith(lineHeader, "v"))
        {
            fscanf(fp, "%f %f %f\n", val, val + 1, val + 2);
            vertexBuffer.emplace_back(val);
            vertCount++;
        }
        // Texture Coordinate
        else if (Utils::startsWith(lineHeader, "vt"))
        {
            fscanf(fp, "%f %f\n", val, val + 1);
            uvBuffer.emplace_back(val);
            uvCount++;
        }
        // Vertex Normal
        else if (Utils::startsWith(lineHeader, "vn"))
        {
            //float val[3];
            fscanf(fp, "%f %f %f\n", val, val + 1, val + 2);
            normBuffer.emplace_back(val);
            normCount++;
        }
        // Face Index
        else if (Utils::startsWith(lineHeader, "f"))
        {
            parseFace(fp);
        }
        // Comment
        else if (Utils::startsWith(lineHeader, "#"))
        {
            // Skip comments
            fscanf(fp, "%[^\r\n]", &buff);
        }
        // Others
        else
        {
            // skip everything except \n or \r
            fscanf(fp, "%[^\r\n]", &buff);
        }
    }
    fclose(fp);
}

void ObjLoader::parseFace(std::FILE* fp)
{
    int err;
    char buff[256] = {};
    int32_t indices[3];
    char endflg;

    err = fscanf(fp, "%s", &buff);
    indices[1] = indices[2] = 0;
    index_t ft = facetype(buff, indices);
    auto addIndex = [](std::vector<uint32_t> &indices, int32_t id,
                       size_t bufferSize, size_t bufferOffset)
    {
        indices.push_back(id > 0 ? id - bufferOffset - 1
                                 : bufferSize + id);
    };

    addIndex(faceIndexBuffer, indices[0], vertCount, vertOffest);
    int count = 1;
    endflg = fgetc(fp);
    switch (ft)
    {
    case VTN://111
        addIndex(uvIndexBuffer, indices[1], uvCount, uvOffest);
        addIndex(normIndexBuffer, indices[2], normCount, normOffest);
        while (endflg != '\n' && endflg != '\r' && endflg != '\0')
        {
            ungetc(endflg, fp);
            fscanf(fp, "%d/%d/%d", indices, indices + 1, indices + 2);
            addIndex(faceIndexBuffer, indices[0], vertCount, vertOffest);
            addIndex(uvIndexBuffer, indices[1], uvCount, uvOffest);
            addIndex(normIndexBuffer, indices[2], normCount, normOffest);
            count++;
            endflg = fgetc(fp);
        }
        faceSizeBuffer.push_back(count);
        break;
    case VT://011
        uvIndexBuffer.push_back(indices[1] - 1);
        while (endflg != '\n' && endflg != '\r' && endflg != '\0')
        {
            ungetc(endflg, fp);
            fscanf(fp, "%d/%d", indices, indices + 1);
            addIndex(faceIndexBuffer, indices[0], vertCount, vertOffest);
            addIndex(uvIndexBuffer, indices[1], uvCount, uvOffest);
            count++;
            endflg = fgetc(fp);
        }
        faceSizeBuffer.push_back(count);
        break;
    case VN://101
        normIndexBuffer.push_back(indices[2] - 1);
        while (endflg != '\n' && endflg != '\r' && endflg != '\0')
        {
            ungetc(endflg, fp);
            fscanf(fp, "%d//%d", indices, indices + 2);
            addIndex(faceIndexBuffer, indices[0], vertCount, vertOffest);
            addIndex(normIndexBuffer, indices[2], normCount, normOffest);
            count++;
            endflg = fgetc(fp);
        }
        faceSizeBuffer.push_back(count);
        break;
    case V://001
        while (endflg != '\n' && endflg != '\r' && endflg != EOF)
        {
            ungetc(endflg, fp);
            fscanf(fp, "%d", indices);
            addIndex(faceIndexBuffer, indices[0], vertCount, vertOffest);
            count++;
            endflg = fgetc(fp);
        }
        faceSizeBuffer.push_back(count);
        break;
    default:
        break;
    }
}

void ObjLoader::swapBuffers()
{
    std::swap(vertexBuffer, vertexBufferFinal);
    std::swap(uvBuffer, uvBufferFinal);
    std::swap(normBuffer, normBufferFinal);
}

void ObjLoader::clearMeshBuffers()
{
    vertexBufferFinal.clear();
    uvBufferFinal.clear();
    normBufferFinal.clear();

    faceIndexBuffer.clear();
    uvIndexBuffer.clear();
    normIndexBuffer.clear();
    faceSizeBuffer.clear();

    vertOffest += vertCount;
    uvOffest += uvCount;
    normOffest += normCount;

    vertCount = uvCount = normCount = 0;

    texAttr.reset();
    texAttr.reset();
}

Primitive* ObjLoader::finalizeMesh()
{
    // use vertexBufferFinal, fid
    texAttr.reset(uvIndexBuffer.size() == faceIndexBuffer.size()
        ? new TextureAttribute(uvBufferFinal, uvIndexBuffer)
        : new TextureAttribute);
    normAttr.reset(normIndexBuffer.size() == faceIndexBuffer.size()
        ? new NormalAttribute(normBufferFinal, normIndexBuffer)
        : new NormalAttribute);

    return PolyMesh::createPolyMesh(vertexBufferFinal,
                                    faceIndexBuffer,
                                    faceSizeBuffer,
                                    texAttr,
                                    normAttr);
}

Primitive* ObjLoader::load(const std::string &filename)
{
    return nullptr;
}

}
