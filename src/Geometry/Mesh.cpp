#include "Mesh.h"
#include "Core/Utils.h"
#include "Geometry/PolyMesh.h"

namespace Kaguya
{

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

bool objFileParser::parse(const char*        filename,
                          vector<Point3f>   &verts,
                          vector<Point2f>   &uvs,
                          vector<Normal3f>  &norms,
                          vector<PolyIndex> &polys)
{
#ifdef _DEBUG
    clock_t start, end;//Timer
    start = clock();
#endif

    FILE* fp = std::fopen(filename, "r");
    if (fp == nullptr)
    {
        return false;
    }
    int err;
    char buff[256] = {};
    char lineHeader[3] = {};
    Float val[3] = {};
    uint32_t indices[3];
    char endflg;

    while (true)
    {
        lineHeader[0] = lineHeader[1] = 0;
        err = fscanf(fp, "%2s", &lineHeader);
        if (err == EOF)
        {
            break;
        }
        // Vertex
        if (strcmp(lineHeader, "v") == 0)
        {
            fscanf(fp, "%f %f %f\n", val, val + 1, val + 2);
            verts.push_back(Point3f(val));
        }
        // Texture Coordinate
        else if (strcmp(lineHeader, "vt") == 0)
        {
            fscanf(fp, "%f %f\n", val, val + 1);
            uvs.push_back(Point2f(val));
        }
        // Vertex Normal
        else if (strcmp(lineHeader, "vn") == 0)
        {
            //float val[3];
            fscanf(fp, "%f %f %f\n", val, val + 1, val + 2);
            norms.push_back(Normal3f(val));
        }
        // Face Index
        else if (strcmp(lineHeader, "f") == 0)
        {
            PolyIndex fid;
            err = fscanf(fp, "%s", &buff);
            indices[1] = indices[2] = 0;
            index_t ft = facetype(buff, indices);
            fid.push_back(indices);
            endflg = fgetc(fp);
            switch (ft)
            {
            case VTN://111
                while (endflg != '\n' && endflg != '\r' && endflg != '\0')
                {
                    ungetc(endflg, fp);
                    fscanf(fp, "%d/%d/%d", indices, indices + 1, indices + 2);
                    fid.push_back(indices);
                    endflg = fgetc(fp);
                }
                break;
            case VT://011
                while (endflg != '\n' && endflg != '\r' && endflg != '\0')
                {
                    ungetc(endflg, fp);
                    fscanf(fp, "%d/%d", indices, indices + 1);
                    fid.push_back(indices);
                    endflg = fgetc(fp);
                }
                break;
            case VN://101
                while (endflg != '\n' && endflg != '\r' && endflg != '\0')
                {
                    ungetc(endflg, fp);
                    fscanf(fp, "%d//%d", indices, indices + 2);
                    fid.push_back(indices);
                    endflg = fgetc(fp);
                }
                break;
            case V://001
                while (endflg != '\n' && endflg != '\r' && endflg != EOF)
                {
                    ungetc(endflg, fp);
                    fscanf(fp, "%d", indices);
                    fid.push_back(indices);
                    endflg = fgetc(fp);
                }
                break;
            default:
                break;
            }
            fid.size = fid.v.size();
            polys.push_back(fid);
        }
        // Comment
        else if (strcmp(lineHeader, "#") == 0)
        {
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

#ifdef _DEBUG
    end = clock();
    std::cout << "OBJ File " << filename << " Loding Time:" << (Float)(end - start) / CLOCKS_PER_SEC << "s" << std::endl;//Timer
#endif // _DEBUG

    return true;
}

bool objFileParser::parse(const char*       filename,
                          vector<Point3f>  &verts,
                          vector<Point2f>  &uvs,
                          vector<Normal3f> &norms,
                          vector<uint32_t> &faceId,
                          vector<uint32_t> &texcoordId,
                          vector<uint32_t> &normId,
                          vector<uint32_t> &faceCount)
{
    std::FILE* fp = std::fopen(filename, "r");
    if (fp == nullptr)
    {
        return false;
    }
    int err;
    char buff[256] = {};
    char lineHeader[3] = {};
    Float val[3] = {};
    uint32_t indices[3];
    char endflg;

    while (true)
    {
        lineHeader[0] = lineHeader[1] = 0;
        err = fscanf(fp, "%2s", &lineHeader);
        if (err == EOF)
        {
            break;
        }
        // Vertex
        if (strcmp(lineHeader, "v") == 0)
        {
            fscanf(fp, "%f %f %f\n", val, val + 1, val + 2);
            verts.emplace_back(val);
        }
        // Texture Coordinate
        else if (strcmp(lineHeader, "vt") == 0)
        {
            fscanf(fp, "%f %f\n", val, val + 1);
            uvs.emplace_back(val);
        }
        // Vertex Normal
        else if (strcmp(lineHeader, "vn") == 0)
        {
            //float val[3];
            fscanf(fp, "%f %f %f\n", val, val + 1, val + 2);
            norms.emplace_back(val);
        }
        // Face Index
        else if (strcmp(lineHeader, "f") == 0)
        {
            //PolyIndex fid;
            err = fscanf(fp, "%s", &buff);
            indices[1] = indices[2] = 0;
            index_t ft = facetype(buff, indices);
            faceId.push_back(indices[0]);
            int count = 1;
            endflg = fgetc(fp);
            switch (ft)
            {
            case VTN://111
                count++;
                texcoordId.push_back(indices[1]);
                normId.push_back(indices[2]);
                while (endflg != '\n' && endflg != '\r' && endflg != '\0')
                {
                    ungetc(endflg, fp);
                    fscanf(fp, "%d/%d/%d", indices, indices + 1, indices + 2);
                    faceId.push_back(indices[0]);
                    texcoordId.push_back(indices[1]);
                    normId.push_back(indices[2]);
                    count++;
                    endflg = fgetc(fp);
                }
                faceCount.push_back(count);
                break;
            case VT://011
                count++;
                texcoordId.push_back(indices[1]);
                while (endflg != '\n' && endflg != '\r' && endflg != '\0')
                {
                    ungetc(endflg, fp);
                    fscanf(fp, "%d/%d", indices, indices + 1);
                    faceId.push_back(indices[0]);
                    texcoordId.push_back(indices[1]);
                    count++;
                    endflg = fgetc(fp);
                }
                faceCount.push_back(count);
                break;
            case VN://101
                count++;
                normId.push_back(indices[2]);
                while (endflg != '\n' && endflg != '\r' && endflg != '\0')
                {
                    ungetc(endflg, fp);
                    fscanf(fp, "%d//%d", indices, indices + 2);
                    faceId.push_back(indices[0]);
                    normId.push_back(indices[2]);
                    count++;
                    endflg = fgetc(fp);
                }
                faceCount.push_back(count);
                break;
            case V://001
                count++;
                while (endflg != '\n' && endflg != '\r' && endflg != EOF)
                {
                    ungetc(endflg, fp);
                    fscanf(fp, "%d", indices);
                    faceId.push_back(indices[0]);
                    count++;
                    endflg = fgetc(fp);
                }
                faceCount.push_back(count);
                break;
            default:
                break;
            }
        }
        // Comment
        else if (strcmp(lineHeader, "#") == 0)
        {
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
    return false;
}

Mesh* createMesh(const std::string &filename, MeshType meshType)
{
    vector<Point3f>   vertexBuffer;
    vector<Point2f>   textureCoords;
    vector<Normal3f>  norms;
    vector<uint32_t>  faceIndexBuffer;
    vector<uint32_t>  texcoordsIndexBuffer;
    vector<uint32_t>  normIndexBuffer;
    vector<uint32_t>  faceCount;
    TextureAttribute* texAttr;
    NormalAttribute*  normAttr;
    if (Utils::endsWith(filename, "obj"))
    {
        if (!objFileParser::parse(filename.c_str(),
                                  vertexBuffer,
                                  textureCoords,
                                  norms,
                                  faceIndexBuffer,
                                  texcoordsIndexBuffer,
                                  normIndexBuffer,
                                  faceCount))
        {
            return nullptr;
        }
    }
    /*else if (Utils::endsWith(filename, "ply"))
    {
    }*/
    texAttr = texcoordsIndexBuffer.size() == faceIndexBuffer.size()
        ? new TextureAttribute(textureCoords, texcoordsIndexBuffer)
        : new TextureAttribute;
    normAttr = normIndexBuffer.size() == faceIndexBuffer.size()
        ? new NormalAttribute(norms, normIndexBuffer)
        : new NormalAttribute;
    if (meshType == MeshType::POLYGONAL_MESH)
    {
        return PolyMesh::createPolyMesh(vertexBuffer,
                                        faceIndexBuffer,
                                        faceCount,
                                        texAttr,
                                        normAttr);
    }
    else if (meshType == MeshType::SUBDIVISION_MESH)
    {
        
    }
    

    return nullptr;
}

}
