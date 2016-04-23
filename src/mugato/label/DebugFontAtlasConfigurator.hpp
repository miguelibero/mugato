#ifndef __mugato__DebugFontAtlas__
#define __mugato__DebugFontAtlas__

#include <buffer.hpp>
#include <string>
#include <mugato/label/FontAtlas.hpp>

namespace gorn
{
    template<typename A>
    class AssetManager;
    class MaterialManager;
    class FileManager;
}

namespace mugato
{
    class Context;

    class DebugFontAtlasConfigurator
    {
    private:
        std::string _name;

        static buffer createBuffer();
    public:
        DebugFontAtlasConfigurator(
            const std::string& name="debug_font");

        void setup(Context& ctx);
        void setup(
            gorn::AssetManager<FontAtlas>& atlases,
            gorn::MaterialManager& materials,
            gorn::FileManager& files);
    };
}

#endif
