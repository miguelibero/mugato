
#include <mugato/sprite/MaterialSpriteAtlasLoader.hpp>
#include <mugato/sprite/SpriteAtlas.hpp>
#include <mugato/sprite/SpriteAtlasRegion.hpp>
#include <mugato/sprite/Sprite.hpp>
#include <gorn/gl/MaterialManager.hpp>
#include <gorn/gl/Material.hpp>

namespace mugato
{

    MaterialSpriteAtlasLoader::MaterialSpriteAtlasLoader
        (gorn::MaterialManager& materials):
        _materials(materials)
    {
    }

    bool MaterialSpriteAtlasLoader::validate(const std::string& name) const NOEXCEPT
    {
        return _materials.validate(name);
    }

    SpriteAtlas MaterialSpriteAtlasLoader::load(const std::string& name) const
    {
        auto size = _materials.loadSize(name);
        SpriteAtlasRegion region;
        region.setSize(size);
        SpriteAtlas atlas;
        atlas.setMaterial(name);
        atlas.addRegion(Sprite::kDefaultFrame, region);
        return atlas;
    }
}

