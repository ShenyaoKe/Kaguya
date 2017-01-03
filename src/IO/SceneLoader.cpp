#include "SceneLoader.h"

namespace Kaguya
{

SceneLoader::SceneLoader(const std::string &filename)
{
}


SceneLoader::~SceneLoader()
{
}

Scene* SceneLoader::load(const std::string &filename)
{
    SceneLoader loader(filename);

    return nullptr;
}

}
