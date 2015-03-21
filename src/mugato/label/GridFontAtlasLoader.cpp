
#include <mugato/label/GridFontAtlasLoader.hpp>
#include <mugato/label/FontAtlas.hpp>
#include <gorn/render/MaterialManager.hpp>
#include <gorn/render/Material.hpp>
#include <gorn/base/Exception.hpp>

namespace mugato {

    GridFontAtlasLoader::GridFontAtlasLoader(
        gorn::MaterialManager& materials,
        const glm::vec2& regionSize,
        char initialChar):
        _materials(materials), _regionSize(regionSize),
        _initialCharacter(initialChar), _advanceDifference(0.0f)
    {
    }

    void GridFontAtlasLoader::setInitialCharacter(char chr) NOEXCEPT
    {
        _initialCharacter = chr;
    }

    void GridFontAtlasLoader::setAdvanceDifference(float val) NOEXCEPT
    {
        _advanceDifference = val;
    }

    bool GridFontAtlasLoader::validate(const std::string& name) const NOEXCEPT
    {
        return _materials.validate(name);
    }

    FontAtlas GridFontAtlasLoader::load(const std::string& name) const
    {
        FontAtlas atlas;
        atlas.setMaterial(name);
        auto msize = _materials.loadSize(name);

        char chr = _initialCharacter;
        for(float y=msize.y-_regionSize.y; y>0; y-=_regionSize.y)
        {
            for(float x=0; x<msize.x; x+=_regionSize.x)
            {
                FontAtlas::Region r;
                r.setSize(_regionSize);
                r.setPosition(glm::vec2(x, y));
                r.setAdvance(_regionSize.x+_advanceDifference);
                atlas.setRegion(std::string()+chr, r);
                chr++;
            }
        }
        if(!atlas.hasRegion(" "))
        {
            FontAtlas::Region r;
            r.setAdvance(_regionSize.x+_advanceDifference);
            atlas.setRegion(" ", r); 
        }
        return atlas;
    }


}
