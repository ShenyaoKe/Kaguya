#include "SceneLoader.h"

#include "Camera/PerspectiveCamera.h"
#include "Camera/OrthographicCamera.h"
#include "Geometry/Mesh.h"

namespace Kaguya
{

SceneLoader::SceneLoader(const std::string &filename)
{
    if (filename.empty())
    {
        return;
    }
    /*FILE* fp = fopen(filename.c_str(), "rb");
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document d;
    d.ParseStream(is);
    fclose(fp);*/

    std::ifstream ifs(filename, std::ifstream::in);
    std::stringstream strStream;
    strStream << ifs.rdbuf(); // 1
    ifs.close();

    for (size_t i = 0; i < filename.size(); i++)
    {
        char curChar = *(filename.end() - i - 1);
#ifdef _WIN32// || _WIN64
        if (curChar == '/' || curChar == '\\')
#else
        if (curChar == '/')
#endif
    	{
            mFilePath = std::string(filename.begin(), filename.end() - i);
            break;
    	}
    }

    mDocument.Parse(strStream.str().c_str());
}

SceneLoader::~SceneLoader()
{
}

Scene* SceneLoader::load(const std::string &filename)
{
    SceneLoader loader(filename);
    std::shared_ptr<Camera> camPtr;
    std::vector<std::shared_ptr<Primitive>> primArray;
    if (loader.mDocument.HasMember("camera"))
    {
        auto &jsonCamera = loader.mDocument.FindMember("camera")->value;
        if (jsonCamera.IsObject())
        {
            camPtr = loader.loadCamera(jsonCamera);
        }
        else if (jsonCamera.IsArray())
        {
            // get all cameras
        }
    }
    if (loader.mDocument.HasMember("primitives"))
    {
        for (auto &prim : loader.mDocument["primitives"].GetArray())
        {
            primArray.push_back(loader.loadPrimitive(prim));
        }
    }
    return new Scene(camPtr, primArray);
}

std::shared_ptr<Camera> SceneLoader::loadCamera(const rapidjson::Value &jsonCamera) const
{
    //const auto &jsonCamera = jsonCamera;// .GetObject();

    std::shared_ptr<Camera> retCamPtr;
    if (jsonCamera.HasMember("type"))
    {
        auto typeStr = jsonCamera["type"].GetString();
        if (!strcmp(typeStr, "perspective"))
        {
            retCamPtr.reset(new PerspectiveCamera);
        }
        else if (!strcmp(typeStr, "orthographic"))
        {
            //retCamPtr.reset(new OrthographicCamera);
        }
        else
        {
        }
    }
    if (jsonCamera.HasMember("resolution"))
    {
        auto &film = retCamPtr->getFilm();
        film.setResolution(jsonCamera["resolution"][0].GetUint(),
                           jsonCamera["resolution"][1].GetUint());
    }
    if (jsonCamera.HasMember("transform"))
    {
        const auto &xform = jsonCamera["transform"];
        if (xform.HasMember("matrix"))
        {
            // direct form transforming matrix
        }
        else
        {
            Point3f eye;
            Point3f targ;
            Vector3f up;
            if (xform.HasMember("position"))
            {
                eye = Point3f(xform["position"][0].GetFloat(),
                              xform["position"][1].GetFloat(),
                              xform["position"][2].GetFloat());
            }
            if (xform.HasMember("target"))
            {
                targ = Point3f(xform["target"][0].GetFloat(),
                               xform["target"][1].GetFloat(),
                               xform["target"][2].GetFloat());
            }
            if (xform.HasMember("up"))
            {
                up = Vector3f(xform["up"][0].GetFloat(),
                              xform["up"][1].GetFloat(),
                              xform["up"][2].GetFloat());
            }
            static_cast<ProjectiveCamera*>(retCamPtr.get())->lookAt(eye, targ, up);
        }
        
    }
    return retCamPtr;
}

std::shared_ptr<Primitive> SceneLoader::loadPrimitive(const rapidjson::Value &jsonCamera) const
{
    std::shared_ptr<Primitive> retPrimPtr;
    if (jsonCamera.HasMember("type"))
    {
        auto typeStr = jsonCamera["type"].GetString();
        if (!strcmp(typeStr, "mesh"))
        {
            MeshType meshType = MeshType::POLYGONAL_MESH;
            uint32_t subdivLevel = 1;
            if (jsonCamera.HasMember("is_subdiv"))
            {
                if (jsonCamera["is_subdiv"].GetBool())
                {
                    meshType = MeshType::POLYGONAL_MESH;
                    if (jsonCamera.HasMember("subdiv_level"))
                    {

                        subdivLevel = jsonCamera["subdiv_level"].GetUint();
                    }
                }
            }
            if (jsonCamera.HasMember("file"))
            {
                retPrimPtr.reset(createMesh(mFilePath + jsonCamera["file"].GetString(),
                               meshType));
            }
        }
        if (!strcmp(typeStr, "curves"))
        {
        }
    }
    return retPrimPtr;
}

}
