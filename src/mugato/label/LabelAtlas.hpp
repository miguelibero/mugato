

#ifndef __mugato__LabelAtlas__
#define __mugato__LabelAtlas__

#include <map>
#include <vector>
#include <string>
#include <mugato/label/LabelAtlasRegion.hpp>

namespace mugato
{
    class LabelAtlas
    {
    public:
        typedef LabelAtlasRegion Region;
    private:
        std::vector<std::string> _materials;
        std::map<std::string, Region> _regions;
        std::map<std::string, std::string> _properties;
    public:
        LabelAtlas();

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
