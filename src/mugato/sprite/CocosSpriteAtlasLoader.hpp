
#ifndef __mugato__CocosSpriteAtlasLoader__
#define __mugato__CocosSpriteAtlasLoader__

#include <gorn/asset/DataAssetLoader.hpp>

namespace mugato {

    class SpriteAtlas;

    class CocosSpriteAtlasLoader : public gorn::DataAssetLoader<SpriteAtlas>
    {
    public:
        CocosSpriteAtlasLoader();

        bool validate(const buffer& data) const NOEXCEPT;
        SpriteAtlas load(const buffer& data) const;

    };
}

#endif
