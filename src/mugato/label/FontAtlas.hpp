

#ifndef __mugato__FontAtlas__
#define __mugato__FontAtlas__

#include <map>
#include <vector>
#include <string>
#include <mugato/label/FontAtlasRegion.hpp>

namespace mugato
{
    class FontAtlas
    {
    public:
        typedef FontAtlasRegion Region;
    private:
        std::vector<std::string> _materials;
        std::map<std::string, Region> _regions;
        std::map<std::string, std::string> _properties;
    public:
        FontAtlas();

        const std::string& getMaterial(size_t page=0) const;
        void setMaterial(const std::string& value, size_t page=0);
        size_t getMaterialCount() const;

        void setRegion(const std::string& name, const Region& region);
        const std::map<std::string, Region>& getRegions() const;
        const Region& getRegion(const std::string& name) const;
        bool hasRegion(const std::string& name) const;
    };

}

#endif
