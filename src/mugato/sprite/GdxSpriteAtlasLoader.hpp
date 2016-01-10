
#ifndef __mugato__GdxSpriteAtlasLoader__
#define __mugato__GdxSpriteAtlasLoader__

#include <gorn/asset/DataAssetLoader.hpp>
#include <mugato/sprite/SpriteAtlas.hpp>

namespace mugato {

    class GdxSpriteAtlasLoader : public gorn::DataAssetLoader<SpriteAtlas>
    {      
    public:
        GdxSpriteAtlasLoader();

        bool validate(const buffer& data) const NOEXCEPT;
        SpriteAtlas load(const buffer& data) const;
    };
}

#endif
