
#include <mugato/sprite/CocosSpriteAtlasLoader.hpp>
#include <mugato/sprite/SpriteAtlas.hpp>
#include <gorn/base/String.hpp>
#include <gorn/render/MaterialManager.hpp>
#include <rapidxml/rapidxml.hpp>
#include <buffer.hpp>
#include <buffer_writer.hpp>

using namespace rapidxml;

namespace mugato {

    void loadXmlDocument(rapidxml::xml_document<>& doc, const buffer& data)
    {
        buffer temp;
        buffer_writer output(temp);
        output.write(data);
        output.fill(0);
        doc.parse<0>(reinterpret_cast<char*>(temp.data()));
    }

    std::vector<int> loadIntegerParts(const std::string& value)
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

    void loadFrame(xml_node<>* dict, SpriteAtlasRegion& region)
    {
        auto key = dict->first_node("key");
        while(key != nullptr)
        {
            std::string name = key->value();
            if(name == "frame")
            {
                std::string value = key->next_sibling()->value();
                std::vector<int> parts = loadIntegerParts(value);
                if(parts.size() != 4)
                {
                    throw std::runtime_error("Invalid frame value.");
                }
                region.setPosition(parts[0], parts[1]);
                region.setSize(parts[2], parts[3]);

            }
            else if(name == "offset")
            {
                std::string value = key->next_sibling()->value();
                std::vector<int> parts = loadIntegerParts(value);
                if(parts.size() != 2)
                {
                    throw std::runtime_error("Invalid offset value.");
                }
                region.setOffset(parts[0], parts[1]);
            }
            else if(name == "rotated")
            {                
                std::string value = key->next_sibling()->name();
                region.setRotate(value == "true");
            }
            else if(name == "sourceSize")                
            {
                std::string value = key->next_sibling()->value();
                std::vector<int> parts = loadIntegerParts(value);
                if(parts.size() != 2)
                {
                    throw std::runtime_error("Invalid sourceSize value.");
                }
                region.setOriginalSize(parts[0], parts[1]);
            }
            key = key->next_sibling("key");
        }
        // in cocos2d offset origin is the center
        region.getOffset() += (region.getOriginalSize()-region.getSize())/2.0f;
    }

    void loadFrames(xml_node<>* dict, SpriteAtlas& atlas)
    {
        auto key = dict->first_node("key");
        while(key != nullptr)
        {
            SpriteAtlasRegion region;
            loadFrame(key->next_sibling("dict"), region);
            atlas.addRegion(key->value(), region);
            key = key->next_sibling("key");
        }
    }

    void loadMetadata(xml_node<>* dict, SpriteAtlas& atlas)
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
        loadXmlDocument(doc, data);
        return std::string(doc.first_node()->name()) == "plist";
    }

    SpriteAtlas CocosSpriteAtlasLoader::load(const buffer& data) const
    {
        xml_document<> doc;
        loadXmlDocument(doc, data);
        
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
                    loadFrames(key->next_sibling("dict"), atlas);
                }
                if(name == "metadata")
                {
                    loadMetadata(key->next_sibling("dict"), atlas);
                }
                key = key->next_sibling("key");
            }
            dict = dict->next_sibling("dict");
        }
        return atlas;
    }

}
