
#include <mugato/sprite/CocosSpriteAtlasLoader.hpp>
#include <mugato/sprite/SpriteAtlas.hpp>
#include <gorn/base/String.hpp>
#include <rapidxml/rapidxml.hpp>
#include <buffer.hpp>

using namespace rapidxml;

namespace mugato
{

    std::vector<int> CocosSpriteAtlasLoadIntegerParts(const std::string& value)
    {
        std::vector<std::string> strparts = gorn::String::split(value, ",");
        std::vector<int> parts;
        for(std::string& strpart : strparts)
        {
            gorn::String::trim(strpart, "{}");
            parts.push_back(gorn::String::convertTo<int>(strpart));
        }
        return parts;
    }

    void CocosSpriteAtlasLoadFrame(xml_node<>* dict, SpriteAtlasRegion& region)
    {
        auto key = dict->first_node("key");
        while(key != nullptr)
        {
            std::string name = key->value();
            if(name == "frame")
            {
                std::string value = key->next_sibling()->value();
                std::vector<int> parts = CocosSpriteAtlasLoadIntegerParts(value);
                if(parts.size() != 4)
                {
                    throw std::runtime_error("Invalid frame value.");
                }
                region.setPosition((float)parts[0], (float)parts[1]);
                region.setSize((float)parts[2], (float)parts[3]);

            }
            else if(name == "offset")
            {
                std::string value = key->next_sibling()->value();
                std::vector<int> parts = CocosSpriteAtlasLoadIntegerParts(value);
                if(parts.size() != 2)
                {
                    throw std::runtime_error("Invalid offset value.");
                }
                region.setOffset((SpriteAtlasRegion::value_type)parts[0], (SpriteAtlasRegion::value_type)parts[1]);
            }
            else if(name == "rotated")
            {                
                std::string value = key->next_sibling()->name();
                region.setRotate(value == "true");
            }
            else if(name == "sourceSize")                
            {
                std::string value = key->next_sibling()->value();
                std::vector<int> parts = CocosSpriteAtlasLoadIntegerParts(value);
                if(parts.size() != 2)
                {
                    throw std::runtime_error("Invalid sourceSize value.");
                }
                region.setOriginalSize((SpriteAtlasRegion::value_type)parts[0], (SpriteAtlasRegion::value_type)parts[1]);
            }
            key = key->next_sibling("key");
        }
        // in cocos2d offset origin is the center
        region.getOffset() += (region.getOriginalSize()-region.getSize())/2.0f;
    }

    void CocosSpriteAtlasLoadFrames(xml_node<>* dict, SpriteAtlas& atlas)
    {
        auto key = dict->first_node("key");
        while(key != nullptr)
        {
            SpriteAtlasRegion region;
            CocosSpriteAtlasLoadFrame(key->next_sibling("dict"), region);
            atlas.addRegion(key->value(), region);
            key = key->next_sibling("key");
        }
    }

    void CocosSpriteAtlasLoadMetadata(xml_node<>* dict, SpriteAtlas& atlas)
    {
        auto key = dict->first_node("key");
        while(key != nullptr)
        {
            std::string name = key->value();
            if(name == "textureFileName")
            {
                auto value = key->next_sibling();
                atlas.setMaterial(value->value());
            }
            key = key->next_sibling("key");
        }
    }

    CocosSpriteAtlasLoader::CocosSpriteAtlasLoader()
    {
    }
    
    bool CocosSpriteAtlasLoader::validate(const buffer& data) const NOEXCEPT
    {
        if(data.binary())
        {
            return false;
        }
        xml_document<> doc;
		doc.parse<0>(const_cast<char*>(data.c_str()));
        return std::string(doc.first_node()->name()) == "plist";
    }

    SpriteAtlas CocosSpriteAtlasLoader::load(const buffer& data) const
    {
        xml_document<> doc;
		doc.parse<0>(const_cast<char*>(data.c_str()));
        
        SpriteAtlas atlas;
        auto root = doc.first_node("plist");
        auto dict = root->first_node("dict");
        while (dict != nullptr)
        {
            auto key = dict->first_node("key");
            while(key != nullptr)
            {
                std::string name = key->value();
                if(name == "frames")
                {
                    CocosSpriteAtlasLoadFrames(key->next_sibling("dict"), atlas);
                }
                if(name == "metadata")
                {
                    CocosSpriteAtlasLoadMetadata(key->next_sibling("dict"), atlas);
                }
                key = key->next_sibling("key");
            }
            dict = dict->next_sibling("dict");
        }
        return atlas;
    }

}
