
#ifndef __mugato__MaterialSpriteAtlasLoader__

#include <gorn/asset/AssetLoader.hpp>

namespace gorn {

    class MaterialManager;
}

namespace mugato {

    class SpriteAtlas;

    class MaterialSpriteAtlasLoader : public gorn::AssetLoader<SpriteAtlas>
    {
    private:
        gorn::MaterialManager& _materials;
    public:
        MaterialSpriteAtlasLoader(gorn::MaterialManager& materials);
        bool validate(const std::string& name) const NOEXCEPT;
        SpriteAtlas load(const std::string& name) const;
    };
}

#endif
