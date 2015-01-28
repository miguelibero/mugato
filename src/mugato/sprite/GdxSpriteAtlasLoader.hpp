
#ifndef __mugato__GdxSpriteAtlasLoader__
#define __mugato__GdxSpriteAtlasLoader__

#include <gorn/asset/AssetLoader.hpp>

namespace mugato {

    class SpriteAtlas;

    class GdxSpriteAtlasLoader : public gorn::AssetLoader<SpriteAtlas>
    {      
    public:
        GdxSpriteAtlasLoader();

        bool validate(const gorn::Data& data) const;
        SpriteAtlas load(gorn::Data&& data) const;
    };
}

#endif
