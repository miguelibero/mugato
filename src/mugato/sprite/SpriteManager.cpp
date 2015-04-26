

#include <mugato/sprite/SpriteManager.hpp>
#include <mugato/sprite/SpriteAtlas.hpp>
#include <mugato/sprite/Sprite.hpp>
#include <gorn/asset/FileManager.hpp>
#include <gorn/gl/MaterialManager.hpp>

namespace mugato {

    SpriteManager::SpriteManager(gorn::MaterialManager& materials, gorn::FileManager& files):
    _materials(materials), _atlases(files)
    {
        getDefinitions().set([](const std::string& name){
            return Definition()
                .withAtlas(name);
        });
    }

    const SpriteManager::Atlases& SpriteManager::getAtlases() const
    {
        return _atlases;
    }

    SpriteManager::Atlases& SpriteManager::getAtlases()
    {
        return _atlases;
    }

    const SpriteManager::Definitions& SpriteManager::getDefinitions() const
    {
        return _definitions;
    }

    SpriteManager::Definitions& SpriteManager::getDefinitions()
    {
        return _definitions;
    }

    SpriteManager::FrameList SpriteManager::loadFrames(
        const std::string& aname, const std::string& fname)
    {
        auto& frames = loadFrames(aname);
        auto itr = frames.find(fname);
        if(itr != frames.end())
        {
            return itr->second;
        }
        else
        {
            return FrameList();
        }
    }

    const SpriteManager::FrameMap& SpriteManager::loadFrames(
            const std::string& aname)
    {
        auto itr = _frames.find(aname);
        if(itr == _frames.end())
        {
            itr = _frames.insert(itr, {aname, FrameMap()});
            auto& frames = itr->second;
            auto atlas = _atlases.load(aname).get();
            auto& regions = atlas->getRegions();
            for(auto ritr = regions.begin(); ritr != regions.end(); ++ritr)
            {
                auto& fname = ritr->first;
                auto fitr = frames.find(fname);
                if(fitr == frames.end())
                {
                    fitr = frames.insert(fitr, {fname, FrameList()});
                    fitr->second.reserve(ritr->second.size());
                    for(auto& region : ritr->second)
                    {
                        std::string mname = atlas->getMaterial(region.getPage());
                        auto material = _materials.load(mname);
                        fitr->second.push_back(std::make_shared<SpriteFrame>(
                            material, region));
                    }

                }
            }
        }

        return itr->second;
    }

    Sprite SpriteManager::load(const std::string& dname)
    {
        auto& def = getDefinitions().get(dname);
        if(def.getAnimations().empty())
        {
            def.setAnimation(Sprite::kDefaultAnimation)
                .withAtlas(dname)
                .withFrame(Sprite::kDefaultFrame);
        }
        Sprite sprite;
        for(auto itr = def.getAnimations().begin();
            itr != def.getAnimations().end(); ++itr)
        {
            auto& anim = sprite.setAnimation(itr->first);
            anim.withFrameDuration(itr->second.getFrameDuration());
            std::string aname = itr->second.getAtlas();
            if(aname.empty())
            {
                aname = def.getAtlas();
            }
            for(auto& fname : itr->second.getFrames())
            {
                auto& frames = loadFrames(aname);
                auto itr = frames.find(fname);
                if(itr != frames.end())
                {
                    anim.withFrames(itr->second);
                }
            }
        }
        return sprite;
    }

}

