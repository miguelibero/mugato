
#include <mugato/label/FntFontAtlasLoader.hpp>
#include <mugato/label/FontAtlas.hpp>
#include <gorn/base/Data.hpp>
#include <gorn/base/String.hpp>

namespace mugato {

    const char* FntSignature = "info ";
    const char FntAttrSepToken = ' ';
    const char FntAttrValSepToken = '=';
    const char FntAttrGroupToken = '"';

    FntFontAtlasLoader::FntFontAtlasLoader()
    {
    }

    bool FntFontAtlasLoader::validate(const gorn::Data& data) const
    {
        gorn::DataInputStream in(data);
        std::string line;
        in.read(line);
        return line.find(FntSignature) == 0;
    }

    std::map<std::string, std::string> parseData(const std::string& str)
    {
        std::size_t p = str.find(FntAttrSepToken);
        std::map<std::string, std::string> map;
        map[""] = str.substr(0, p);
        while(true)
        {
			if(p>=str.size())
			{
				break;
			}
            if(str[p] == FntAttrSepToken)
            {
                p++;
                continue;
            }
            std::size_t n = str.find(FntAttrSepToken, p);
            if(n == std::string::npos)
            {
                n = str.size()-1;
            }
            std::size_t s = str.find(FntAttrValSepToken, p);
            if(s == std::string::npos)
            {
                break;
            }            
            std::string name = str.substr(p, s-p);
            s++;
            if(str[s] == FntAttrGroupToken)
            {
                s++;
                n = str.find(FntAttrGroupToken, s);
                if(n == std::string::npos)
                {
                    break;
                }
            }
            std::string value = str.substr(s, n-s);
            map[name] = value;
            p = n;
        }
        return map;
    }

    FontAtlas FntFontAtlasLoader::load(gorn::Data&& data) const
    {
        FontAtlas atlas;
        gorn::DataInputStream in(data);
        std::vector<std::string> pages;
        while(!in.reachedEnd())
        {
            std::string line;
            in.read(line);
            auto map = parseData(line);
            auto name = map[""];
            map.erase("");
            if(name == "info" || name == "common")
            {
                // TODO: take font properties
            }            
            else if(name == "page")
            {
                auto i = gorn::String::convertTo<size_t>(map["id"]);
                atlas.setMaterial(map["file"], i);
            }
            else if(name == "char")
            {
                FontAtlasRegion region;
                region.setOrigin(SpriteAtlasRegion::Origin::TopLeft);
                region.setPosition(
                    gorn::String::convertTo<SpriteAtlasRegion::value_type>(map["x"]),
                    gorn::String::convertTo<SpriteAtlasRegion::value_type>(map["y"])
                );
                region.setSize(
                    gorn::String::convertTo<SpriteAtlasRegion::value_type>(map["width"]),
                    gorn::String::convertTo<SpriteAtlasRegion::value_type>(map["height"])
                );
                region.setAdvance(
                    gorn::String::convertTo<SpriteAtlasRegion::value_type>(map["xadvance"])
                );

                glm::vec2 offset(
                    gorn::String::convertTo<SpriteAtlasRegion::value_type>(map["xoffset"]),
                    gorn::String::convertTo<SpriteAtlasRegion::value_type>(map["yoffset"])
                );

                region.setOriginalSize(region.getSize()+offset);
                region.setOffset(offset*glm::vec2(0.5f, -0.5f));

                std::string name = map["letter"];
                if(name == "space")
                {
                    name = " ";
                }
                region.setPage(gorn::String::convertTo<size_t>(map["page"]));
                atlas.setRegion(name, region);
            }
        }
        
        return atlas;
    }


}
