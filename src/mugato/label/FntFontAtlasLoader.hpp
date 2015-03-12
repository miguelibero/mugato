
#ifndef __mugato__FntFontAtlasLoader__

#include <gorn/asset/DataAssetLoader.hpp>

namespace mugato {

    class FontAtlas;

    class FntFontAtlasLoader : public gorn::DataAssetLoader<FontAtlas>
    {
    public:
        FntFontAtlasLoader();
        bool validate(const buffer& data) const NOEXCEPT;
        FontAtlas load(const buffer& data) const; 
    };
}

#endif
