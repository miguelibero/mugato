
#include <mugato/label/FntFontAtlasLoader.hpp>
#include <mugato/label/FontAtlas.hpp>
#include <buffer.hpp>
#include <buffer_reader.hpp>
#include <gorn/base/String.hpp>

namespace mugato {

    const char* FntSignature = "info ";
    const char FntAttrSepToken = ' ';
    const char FntAttrValSepToken = '=';
    const char FntAttrGroupToken = '"';

    FntFontAtlasLoader::FntFontAtlasLoader()
    {
    }

    bool FntFontAtlasLoader::validate(const buffer& data) const NOEXCEPT
    {
        if(data.binary())
        {
            return false;
        }
        buffer_reader in(data);
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

    FontAtlas FntFontAtlasLoader::load(const buffer& data) const
    {
        FontAtlas atlas;
        buffer_reader in(data);
        std::vector<std::string> pages;
        while(!in.end())
        {
            std::string line;
            in.read(line);
            auto map = parseData(line);
            auto name = map[""];
            map.erase("");
            float baseline = 0.0f;
            if(name == "info")
            {
                // TODO: take font properties
            }
            else if(name == "common")
            {
                baseline = gorn::String::convertTo<float>(map["base"]);
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
                    gorn::String::convertTo<float>(map["x"]),
                    gorn::String::convertTo<float>(map["y"])
                );
                region.setSize(
                    gorn::String::convertTo<float>(map["width"]),
                    gorn::String::convertTo<float>(map["height"])
                );
                region.setAdvance(
                    gorn::String::convertTo<float>(map["xadvance"])
                );

                glm::vec2 offset(
                    gorn::String::convertTo<float>(map["xoffset"]),
                    gorn::String::convertTo<float>(map["yoffset"])
                );
                offset.y = baseline - region.getSize().y - offset.y ;

                region.setOriginalSize(region.getSize());
                region.setOffset(offset);

                auto itr = map.find("id");
                std::string name;
                if(itr != map.end())
                {
                     name += (char)gorn::String::convertTo<int>(itr->second);
                }
                else
                {
                    name = map["letter"];
                    if(name == "space")
                    {
                        name = " ";
                    }
                }
                region.setPage(gorn::String::convertTo<size_t>(map["page"]));
                atlas.setRegion(name, region);
            }
        }
        
        return atlas;
    }


}
