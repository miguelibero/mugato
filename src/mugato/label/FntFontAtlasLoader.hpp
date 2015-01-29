
#ifndef __mugato__FntFontAtlasLoader__

#include <gorn/asset/DataAssetLoader.hpp>

namespace mugato {

    class FontAtlas;

    class FntFontAtlasLoader : public gorn::DataAssetLoader<FontAtlas>
    {
    public:
        FntFontAtlasLoader();
        bool validate(const gorn::Data& data) const;
        FontAtlas load(gorn::Data&& data) const; 
    };
}

#endif
