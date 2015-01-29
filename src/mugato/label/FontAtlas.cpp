
#include <mugato/label/FontAtlas.hpp>
#include <mugato/base/Exception.hpp>
#include <gorn/base/String.hpp>

namespace mugato {

    FontAtlas::FontAtlas()
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
}
