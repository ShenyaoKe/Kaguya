#pragma once
#include "Core/Scene.h"

namespace Kaguya
{

class SceneLoader
{
public:
    SceneLoader(const std::string &filename);
    ~SceneLoader();
public:
    static Scene* load(const std::string &filename);
private:

};

}
