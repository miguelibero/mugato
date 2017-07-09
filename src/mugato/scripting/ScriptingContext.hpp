
#ifndef __mugato__ScriptingContext__
#define __mugato__ScriptingContext__

#include <string>

struct lua_State;

namespace mugato
{
    class Context;
}

namespace mugato
{
    class Context;

    class ScriptingContext
    {
        lua_State* L;
        gorn::Context& _gorn;
        mugato::Context& _mugato;

        void throwLuaException(const std::string& prefix);

    public:
        ScriptingContext(gorn::Context& gorn, mugato::Context& mugato);
        void load(const std::string& name);
        void update(double dt);
        void unload();
    };
}

#endif
