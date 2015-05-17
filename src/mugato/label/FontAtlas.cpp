
#include <mugato/label/FontAtlas.hpp>
#include <mugato/label/LabelFont.hpp>
#include <mugato/base/Exception.hpp>
#include <gorn/base/String.hpp>
#include <gorn/gl/MaterialManager.hpp>

namespace mugato {

    FontAtlas::FontAtlas():
    _lineHeight(0.0f)
    {
    }

    const std::string& FontAtlas::getMaterial(size_t page) const
    {
        return _materials.at(page);
    }

    void FontAtlas::setMaterial(const std::string& value, size_t page)
    {
        if(_materials.size() <= page)
        {
            _materials.resize(page+1);
        }
        _materials[page] = value;
    }

    size_t FontAtlas::getMaterialCount() const
    {
        return _materials.size();
    }

    void FontAtlas::setRegion(const std::string& name, const Region& region)
    {
        _regions[name] = region;
    }

    const std::map<std::string, FontAtlas::Region>& FontAtlas::getRegions() const
    {
        return _regions;
    }

    const FontAtlas::Region& FontAtlas::getRegion(const std::string& name) const
    {
        auto itr = _regions.find(name);
        if(itr != _regions.end())
        {
            return itr->second;
        }
        throw Exception("Could not find region.");
    }

    bool FontAtlas::hasRegion(const std::string& name) const
    {
        return _regions.find(name) != _regions.end();
    }

    void FontAtlas::setLineHeight(float val)
    {
        _lineHeight = val;
    }

    float FontAtlas::getLineHeight() const
    {
        if(_lineHeight == 0.0f)
        {
            float max = 0.0f;
            for(auto itr = _regions.begin(); itr != _regions.end(); ++itr)
            {
                auto h = itr->second.getOriginalSize().y;
                if(max < h)
                {
                    max = h;
                }
            }
            return max;
        }
        return _lineHeight;
    }

    LabelFont FontAtlas::createFont(gorn::MaterialManager& materials) const
    {
        LabelFont font;
        font.setLineHeight(getLineHeight());
        for(auto itr=_regions.begin(); itr!=_regions.end(); ++itr)
        {
            auto& region = itr->second;
            std::string mname = getMaterial(region.getPage());
            auto material = materials.load(mname);
            LabelFont::Character chr(material, region);
            font.setCharacter(itr->first, chr);
        }
        return font;
    }
}
