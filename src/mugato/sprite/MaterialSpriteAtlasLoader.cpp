
#include <mugato/sprite/MaterialSpriteAtlasLoader.hpp>
#include <mugato/sprite/SpriteAtlas.hpp>
#include <mugato/sprite/SpriteAtlasRegion.hpp>
#include <mugato/sprite/Sprite.hpp>
#include <gorn/render/MaterialManager.hpp>
#include <gorn/render/Material.hpp>

namespace mugato {

    MaterialSpriteAtlasLoader::MaterialSpriteAtlasLoader
        (gorn::MaterialManager& materials):
        _materials(materials)
    {
    }

    bool MaterialSpriteAtlasLoader::validate(const std::string& name) const
    {
        return _materials.validate(name);
    }

    SpriteAtlas MaterialSpriteAtlasLoader::load(const std::string& name) const
    {
        auto material = _materials.load(name);
        SpriteAtlasRegion region;
        region.setSize(material->getSize());
        SpriteAtlas atlas;
        atlas.setMaterial(name);
        atlas.addRegion(Sprite::kDefaultFrame, region);
        return atlas;
    }
}

