#ifndef __mugato__DebugFontAtlas__
#define __mugato__DebugFontAtlas__

#include <buffer.hpp>
#include <string>

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

    class FontAtlas;

    class DebugFontAtlasConfigurator
    {
    private:
        std::string _fileName;
        std::string _tagName;

        static buffer createBuffer();
    public:
        DebugFontAtlasConfigurator(
            const std::string& fileName="debug_font",
            const std::string& tagName="debug");

        void setup(Context& ctx);
        void setup(
            gorn::AssetManager<FontAtlas>& atlases,
            gorn::MaterialManager& materials, 
            gorn::FileManager& files);
    };
}

#endif


