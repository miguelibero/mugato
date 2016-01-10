
#include <mugato/sprite/GdxSpriteAtlasLoader.hpp>
#include <mugato/sprite/SpriteAtlas.hpp>
#include <gorn/base/String.hpp>
#include <buffer.hpp>
#include <buffer_reader.hpp>

namespace mugato {

    GdxSpriteAtlasLoader::GdxSpriteAtlasLoader()
    {
    }
    
    bool GdxSpriteAtlasLoader::validate(const buffer& data) const NOEXCEPT
    {
        return !data.binary();
    }

    SpriteAtlas GdxSpriteAtlasLoader::load(const buffer& data) const
    {
        SpriteAtlas atlas;
        std::string line;
        buffer_reader input(data);

        // texture name
        while(line.empty() && !input.end())
        {
            input.read(line);
            gorn::String::trim(line);
        }
        atlas.setMaterial(line);

        bool inRegion = false;
        SpriteAtlasRegion region;
        std::string regionName;
        size_t regionIndex = 0;

        while(!input.end())
        {
            input.read(line);
            std::size_t sep = line.find(':');
            std::string n = line.substr(0, sep);
            std::string v = line.substr(sep+1);

            if(!inRegion)
            {
                if(sep == std::string::npos)
                {
                    inRegion = true;
                    region = SpriteAtlasRegion();
                    region.setOrigin(SpriteAtlasRegionOrigin::TopLeft);
                    regionName = line;
                }
                else
                {
                    // header value
                }
            }
            else
            {
                if(line.substr(0, 1) == " ")
                {
                    gorn::String::trim(n);
                    gorn::String::trim(v);
                    if(n == "rotate")
                    {
                        region.setRotate(v == "true");
                    }
                    else if(n == "xy")
                    {
                        sep = v.find(',');                       
                        region.setPosition(
                            gorn::String::convertTo<SpriteAtlasRegion::value_type>(v.substr(0, sep)),
                            gorn::String::convertTo<SpriteAtlasRegion::value_type>(v.substr(sep+1)));
                    }
                    else if(n == "size")
                    {
                        sep = v.find(',');
                        region.setSize(
                            gorn::String::convertTo<SpriteAtlasRegion::value_type>(v.substr(0, sep)),
                            gorn::String::convertTo<SpriteAtlasRegion::value_type>(v.substr(sep+1)));
                    }
                    else if(n == "orig")
                    {
                        sep = v.find(',');
                        region.setOriginalSize(
                            gorn::String::convertTo<SpriteAtlasRegion::value_type>(v.substr(0, sep)),
                            gorn::String::convertTo<SpriteAtlasRegion::value_type>(v.substr(sep+1)));
                    }
                    else if(n == "offset")
                    {
                        sep = v.find(',');
                        region.setOffset(
                            gorn::String::convertTo<SpriteAtlasRegion::value_type>(v.substr(0, sep)),
                            gorn::String::convertTo<SpriteAtlasRegion::value_type>(v.substr(sep+1)));
                    }
                    else if(n == "index")
                    {
                        regionIndex = gorn::String::convertTo<size_t>(v);
                    }
                }
                else
                {
                    atlas.setRegion(regionName, region, regionIndex);
                    region = SpriteAtlasRegion();
                    region.setOrigin(SpriteAtlasRegionOrigin::TopLeft);
                    regionName = line;
                }             
            }
        }
        atlas.setRegion(regionName, region, regionIndex);

        return atlas;
    }

}
