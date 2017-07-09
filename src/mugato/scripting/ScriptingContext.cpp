
#include <gorn/base/Context.hpp>
#include <gorn/asset/FileManager.hpp>
#include <mugato/base/Exception.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/scripting/ScriptingContext.hpp>

#include <lua.hpp>

namespace mugato
{
    ScriptingContext::ScriptingContext(gorn::Context& gorn, mugato::Context& mugato):
    L(nullptr), _gorn(gorn), _mugato(mugato)
    {
    }

    void ScriptingContext::throwLuaException(const std::string& prefix)
    {
        std::string msg(lua_tostring(L, -1));
        lua_pop(L, 1);
        throw Exception(prefix+": "+msg);
    }

    static int luaMugatoGetScenes(lua_State *L)
    {
        auto ctx = (Context*)lua_touserdata( L, lua_upvalueindex( 1 ) );
        ctx->getScenes();
        return 0;
    }

    static const luaL_Reg mugatolib[] = {
        {"getScenes",   luaMugatoGetScenes},
        {NULL, NULL}
    };

    const char* LuaRegistryMugato = "mugato";
    const char* LuaRegistryGorn = "gorn";

    void ScriptingContext::load(const std::string& script)
    {
        if(L == nullptr)
        {
            L = luaL_newstate();
            luaL_openlibs(L);
        }

        lua_pushlightuserdata(L, (void *)&LuaRegistryMugato);
        lua_pushlightuserdata(L, (void *)&_mugato);
        lua_settable(L, LUA_REGISTRYINDEX);

        luaL_newlib(L, mugatolib);
        lua_setglobal(L, "mugato");

        auto buffer = _gorn.getFiles().load(script).get();
        auto result = luaL_loadbuffer(L, (const char*)buffer.data(), buffer.size(), script.c_str());
        if( result != LUA_OK )
        {
            throwLuaException("Could not load script");
        }
        result = lua_pcall(L, 0, LUA_MULTRET, 0);
        if( result != LUA_OK )
        {
            throwLuaException("lua_pcall() failed");
        }
    }

    void ScriptingContext::update(double dt)
    {

    }

    void ScriptingContext::unload()
    {
        lua_close(L);
        L = nullptr;
    }

}
