
#include <mugato/spine/SpineManager.hpp>
#include <mugato/spine/SpineSkeleton.hpp>
#include <mugato/base/Exception.hpp>
#include <gorn/gl/MaterialManager.hpp>
#include <gorn/gl/Material.hpp>
#include <gorn/asset/FileManager.hpp>
#include <spine/spine.h>
#include <buffer.hpp>

mugato::SpineManager* s_current = nullptr;

extern "C" {

    void _spAtlasPage_createTexture(spAtlasPage* self, const char* path)
    {
        if(s_current == nullptr)
        {
            throw mugato::Exception("No mugato::SpineManager found.");
        }
        auto material = s_current->getMaterials().load(path);
	    self->rendererObject = new std::shared_ptr<gorn::Material>(material);
        auto& size = material->getSize();
	    self->width = size.x;
	    self->height = size.y;
    }

    void _spAtlasPage_disposeTexture(spAtlasPage* self)
    {
	    delete (std::shared_ptr<gorn::Material>*)self->rendererObject;
    }

    char* _spUtil_readFile(const char* path, int* length)
    {
        if(s_current == nullptr)
        {
            throw mugato::Exception("No mugato::SpineManager found.");
        }
        auto buffer = s_current->getFiles().load(path).get();
        *length = (int)buffer.size()/sizeof(char);
        return reinterpret_cast<char*>(buffer.detach());
    }

}

namespace mugato {

    SpineManager::SpineManager(gorn::MaterialManager& materials,
        gorn::FileManager& files):
    _materials(materials), _files(files)
    {
        getDefinitions().set([](const std::string& name){
            return Definition()
                .withDataFile(name+".json")
                .withAtlasFile(name+".atlas");
        });
    }

    SpineManager::~SpineManager()
    {
    }

    const SpineManager::Definitions& SpineManager::getDefinitions() const
    {
        return _definitions;
    }

    SpineManager::Definitions& SpineManager::getDefinitions()
    {
        return _definitions;
    }

    gorn::MaterialManager& SpineManager::getMaterials()
    {
        return _materials;
    }

    gorn::FileManager& SpineManager::getFiles()
    {
        return _files;
    }

    std::shared_ptr<SpineSkeletonData> SpineManager::loadData(const Definition& def)
    {
        s_current = this;
        auto itr = _atlases.find(def.getAtlasFile());
        std::shared_ptr<SpineAtlas> atlas;
        if(itr == _atlases.end())
        {
            atlas = std::make_shared<SpineAtlas>(def.getAtlasFile());
            _atlases.insert(itr, {def.getAtlasFile(), atlas});
        }
        else
        {
            atlas = itr->second;
        }
        auto data = std::make_shared<SkeletonData>(atlas, def.getDataFile());
        for(auto& mix : def.getAnimationMixes())
        {
            data->setAnimationMix(mix.getFrom(), mix.getTo(), mix.getDuration());
        }

        return data;
    }

    SpineSkeleton SpineManager::load(const std::string& name)
    {
        auto itr = _datas.find(name);
        std::shared_ptr<SkeletonData> data;
        if(itr == _datas.end())
        {
            auto& def = _definitions.get(name);
            data = loadData(def);
            _datas.insert(itr, {name, data});
        }
        else
        {
            data = itr->second;
        }
        return SpineSkeleton(data);
    }   

}
