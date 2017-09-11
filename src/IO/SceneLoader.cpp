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

		if (curChar == '/' || curChar == '\\')
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
	std::vector<std::shared_ptr<RenderPrimitive>> primArray;
	std::vector<std::shared_ptr<Light>> lightArray;
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
			std::shared_ptr<Geometry> retPrim = loader.loadGeometry(prim);
			std::shared_ptr<Light> geomEmission;
			if (retPrim != nullptr)
			{
				primArray.emplace_back(std::make_shared<RenderPrimitive>(retPrim,
																		 geomEmission));
			}
		}
	}

	return new Scene(camPtr, primArray, lightArray);
}

std::shared_ptr<Camera> SceneLoader::loadCamera(const rapidjson::Value &jsonCamera) const
{
	std::shared_ptr<Camera> retCamPtr;
	if (jsonCamera.HasMember("type"))
	{
		auto typeStr = jsonCamera["type"].GetString();
		if (!strcmp(typeStr, "perspective"))
		{
			retCamPtr = std::make_shared<PerspectiveCamera>();
		}
		else if (!strcmp(typeStr, "orthographic"))
		{
			retCamPtr = std::make_shared<OrthographicCamera>();
			if (jsonCamera.HasMember("ortho_scalor"))
			{
				std::static_pointer_cast<OrthographicCamera>(retCamPtr)->setScalor(
					jsonCamera["ortho_scalor"].GetDouble());
			}
		}
		else
		{
		}
	}
	// Set film
	if (jsonCamera.HasMember("resolution"))
	{
		auto &film = retCamPtr->getFilm();
		film.setResolution(jsonCamera["resolution"][0].GetUint(),
						   jsonCamera["resolution"][1].GetUint());
	}
	if (jsonCamera.HasMember("film_type"))
	{
		auto &film = retCamPtr->getFilm();
		film.setFilmType(Kaguya::FILM_TYPE::FT_SQUARE);
	}
	else if (jsonCamera.HasMember("aperture"))
	{
		auto &film = retCamPtr->getFilm();
		film.setAperture(jsonCamera["aperture"][0].GetDouble(),
						 jsonCamera["aperture"][1].GetDouble());
	}
	// Set transform
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

std::shared_ptr<Geometry> SceneLoader::loadGeometry(const rapidjson::Value &jsonCamera) const
{
	std::shared_ptr<Geometry> retPrimPtr;
	if (jsonCamera.HasMember("type"))
	{
		const char* typeStr = jsonCamera["type"].GetString();
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
				const char* filename = jsonCamera["file"].GetString();
				retPrimPtr = createMesh(mFilePath + filename, meshType);
				retPrimPtr->setName(filename);
			}
		}
		else if (!strcmp(typeStr, "curves"))
		{
			// TODO
		}
	}
	return retPrimPtr;
}

}
