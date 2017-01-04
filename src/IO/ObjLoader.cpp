#include "ObjLoader.h"
#include "Geometry/Primitive.h"
#include "Geometry/PolyMesh.h"
#include "Core/Utils.h"

namespace Kaguya
{

ObjLoader::ObjLoader(const std::string &filename)
    : fp(std::fopen(filename.c_str(), "r"))
    , vertCount(0), vertRange{}
    , uvCount(0), uvRange{}
    , normCount(0), normRange{}
    , mFilename(filename)
{
    if (fp != nullptr)
    {
        parse(fp);
    }
    if (!faceIndexBuffer.empty())
    {
        //updateBufferRange();
        mPrims.emplace_back(finalizeMesh());
    }
    fclose(fp);
}

ObjLoader::~ObjLoader()
{
}

ObjLoader::index_t ObjLoader::facetype(const char* str, int32_t* val)
{
    int argv = sscanf(str, "%d/%d/%d%*[ \t]", val, val + 1, val + 2);
    switch (argv)
    {
    case 3:// V/T/N
        return VTN;//111
    case 2:// V/T
        return VT;//011
    case 1:
        argv = sscanf(str, "%d//%d%*[ \t]", val, val + 2);
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
    char lineHeader[16] = {};
    Float val[3] = {};

    while (true)
    {
        lineHeader[0] = lineHeader[1] = 0;
        err = fscanf(fp, "%6s", &lineHeader);
        if (err == EOF)
        {
            break;
        }

        bool isBoundary = Utils::startsWith(lineHeader, "g")
            || Utils::startsWith(lineHeader, "o")
            || Utils::startsWith(lineHeader, "s")
            || Utils::startsWith(lineHeader, "usemtl");

        if (isBoundary && !faceIndexBuffer.empty())
        {
            mPrims.emplace_back(finalizeMesh());
            clearMeshBuffers();
        }


        // Vertex
        if (Utils::startsWith(lineHeader, "v"))
        {
            fscanf(fp, "%f %f %f", val, val + 1, val + 2);
            vertexBuffer.emplace_back(val);
        }
        // Texture Coordinate
        else if (Utils::startsWith(lineHeader, "vt"))
        {
            fscanf(fp, "%f %f", val, val + 1);
            uvBuffer.emplace_back(val);
        }
        // Vertex Normal
        else if (Utils::startsWith(lineHeader, "vn"))
        {
            fscanf(fp, "%f %f %f", val, val + 1, val + 2);
            normBuffer.emplace_back(val);
        }
        // Face Index
        else if (Utils::startsWith(lineHeader, "f"))
        {
            if (faceIndexBuffer.empty())
            {
                updateBufferRange();
            }
            parseFace(fp);
        }
        // Comment
        else if (Utils::startsWith(lineHeader, "#"))
        {
            // Skip comments
            fscanf(fp, "%[^\r\n]", &buff);
        }
        else if (Utils::startsWith(lineHeader, "g"))
        {
            fscanf(fp, "%s", buff);
            mFilename = std::string(buff);
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

    addIndex(faceIndexBuffer, indices[0], vertCount, vertRange[0]);
    int count = 1;
    endflg = fgetc(fp);
    switch (ft)
    {
    case VTN://111
        addIndex(uvIndexBuffer, indices[1], uvCount, uvRange[0]);
        addIndex(normIndexBuffer, indices[2], normCount, normRange[0]);
        while (endflg != '\n' && endflg != '\r' && endflg != EOF)
        {
            ungetc(endflg, fp);
            fscanf(fp, "%d/%d/%d%*[ \t]", indices, indices + 1, indices + 2);
            addIndex(faceIndexBuffer, indices[0], vertCount, vertRange[0]);
            addIndex(uvIndexBuffer, indices[1], uvCount, uvRange[0]);
            addIndex(normIndexBuffer, indices[2], normCount, normRange[0]);
            count++;
            endflg = fgetc(fp);
        }
        faceSizeBuffer.push_back(count);
        break;
    case VT://011
        addIndex(uvIndexBuffer, indices[1], uvCount, uvRange[0]);
        while (endflg != '\n' && endflg != '\r' && endflg != EOF)
        {
            ungetc(endflg, fp);
            fscanf(fp, "%d/%d%*[ \t]", indices, indices + 1);
            addIndex(faceIndexBuffer, indices[0], vertCount, vertRange[0]);
            addIndex(uvIndexBuffer, indices[1], uvCount, uvRange[0]);
            count++;
            endflg = fgetc(fp);
        }
        faceSizeBuffer.push_back(count);
        break;
    case VN://101
        addIndex(normIndexBuffer, indices[2], normCount, normRange[0]);
        while (endflg != '\n' && endflg != '\r' && endflg != EOF)
        {
            ungetc(endflg, fp);
            fscanf(fp, "%d//%d%*[ \t]", indices, indices + 2);
            addIndex(faceIndexBuffer, indices[0], vertCount, vertRange[0]);
            addIndex(normIndexBuffer, indices[2], normCount, normRange[0]);
            count++;
            endflg = fgetc(fp);
        }
        faceSizeBuffer.push_back(count);
        break;
    case V://001
        while (endflg != '\n' && endflg != '\r' && endflg != EOF)
        {
            ungetc(endflg, fp);
            fscanf(fp, "%d%*[ \t]", indices);
            addIndex(faceIndexBuffer, indices[0], vertCount, vertRange[0]);
            count++;
            endflg = fgetc(fp);
        }
        faceSizeBuffer.push_back(count);
        break;
    default:
        break;
    }
}

void ObjLoader::updateBufferRange()
{
    if (vertRange[1] < vertexBuffer.size())
    {
        vertRange[0] = vertRange[1];
        vertRange[1] = vertexBuffer.size();
        vertCount = vertRange[1] - vertRange[0];
    }
    if (uvRange[1] < uvBuffer.size())
    {
        uvRange[0] = uvRange[1];
        uvRange[1] = uvBuffer.size();
        uvCount = uvRange[1] - uvRange[0];
    }
    if (normRange[1] < normBuffer.size())
    {
        normRange[0] = normRange[1];
        normRange[1] = normBuffer.size();
        normCount = normRange[1] - normRange[0];
    }
}

void ObjLoader::clearMeshBuffers()
{
    faceIndexBuffer.clear();
    uvIndexBuffer.clear();
    normIndexBuffer.clear();
    faceSizeBuffer.clear();

    texAttr.reset();
    texAttr.reset();
}

Primitive* ObjLoader::finalizeMesh()
{
    // use vertexBufferFinal, fid
    texAttr.reset(uvIndexBuffer.size() == faceIndexBuffer.size()
                  ? new TextureAttribute(
                      std::vector<Point2f>(uvBuffer.begin() + uvRange[0],
                                           uvBuffer.begin() + uvRange[1]),
                      uvIndexBuffer)
                  : new TextureAttribute);
    normAttr.reset(normIndexBuffer.size() == faceIndexBuffer.size()
                   ? new NormalAttribute(
                       std::vector<Normal3f>(normBuffer.begin() + normRange[0],
                                             normBuffer.begin() + normRange[1]),
                       normIndexBuffer)
                   : new NormalAttribute);

    return PolyMesh::createPolyMesh(std::vector<Point3f>(vertexBuffer.begin() + vertRange[0],
                                                         vertexBuffer.begin() + vertRange[1]),
                                    faceIndexBuffer,
                                    faceSizeBuffer,
                                    texAttr,
                                    normAttr);
}

std::vector<std::shared_ptr<Primitive>> ObjLoader::load(const std::string &filename)
{
    ObjLoader loader(filename);

    return loader.mPrims;
}

}
