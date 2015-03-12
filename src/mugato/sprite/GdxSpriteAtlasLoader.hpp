
#ifndef __mugato__GdxSpriteAtlasLoader__
#define __mugato__GdxSpriteAtlasLoader__

#include <gorn/asset/DataAssetLoader.hpp>

namespace mugato {

    class SpriteAtlas;

    class GdxSpriteAtlasLoader : public gorn::DataAssetLoader<SpriteAtlas>
    {      
    public:
        GdxSpriteAtlasLoader();

        bool validate(const buffer& data) const NOEXCEPT;
        SpriteAtlas load(const buffer& data) const;
    };
}

#endif
