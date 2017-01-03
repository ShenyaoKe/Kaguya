#include "SceneLoader.h"
#include <Lua5.3/lua.hpp>

namespace Kaguya
{

SceneLoader::SceneLoader(const std::string &filename)
{
    lua_State* state = luaL_newstate();
    luaL_openlibs(state);
    if (luaL_loadfile(state, filename.c_str()) != LUA_OK)
    {
        std::cout << "fail to load lua file\n";
        return;
    }

    if (lua_pcall(state, 0, LUA_MULTRET, 0) != LUA_OK) {
        std::cout << "fail to call lua function\n";
        return;
    }
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
