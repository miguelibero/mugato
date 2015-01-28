
#ifndef __mugato__CocosSpriteAtlasLoader__
#define __mugato__CocosSpriteAtlasLoader__

#include <gorn/asset/AssetLoader.hpp>

namespace mugato {

    class SpriteAtlas;

    class CocosSpriteAtlasLoader : public gorn::AssetLoader<SpriteAtlas>
    {
    public:
        CocosSpriteAtlasLoader();

        bool validate(const gorn::Data& data) const;
        SpriteAtlas load(gorn::Data&& data) const;

    };
}

#endif
