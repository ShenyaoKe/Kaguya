#include "SceneLoader.h"
#include <rapidjson/filereadstream.h>
#include <rapidjson/document.h>

namespace Kaguya
{

SceneLoader::SceneLoader(const std::string &filename)
{
    if (filename.empty())
    {
        return;
    }
    FILE* fp = fopen(filename.c_str(), "rb");
    char readBuffer[65536];
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document d;
    d.ParseStream(is);
    fclose(fp);

    d.FindMember("hello");
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
