
#include <mugato/spine/SpineManager.hpp>
#include <mugato/spine/SpineSkeleton.hpp>
#include <mugato/base/Exception.hpp>
#include <gorn/render/MaterialManager.hpp>
#include <gorn/render/Material.hpp>
#include <gorn/asset/FileManager.hpp>
#include <spine/spine.h>
#include <buffer.hpp>

mugato::SpineManager* s_current = nullptr;

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
    *length = buffer.size()/sizeof(char);
    return reinterpret_cast<char*>(buffer.detach());
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

    spSkeletonData* SpineManager::loadData(const Definition& def)
    {
        s_current = this;
        spAtlas* atlas = spAtlas_createFromFile(def.getAtlasFile().c_str(), 0);
        if(atlas == nullptr)
        {
            throw Exception("Error reading atlas data.");
        }
        spSkeletonJson* json = spSkeletonJson_create(atlas);
	    spSkeletonData* data =
            spSkeletonJson_readSkeletonDataFile(json,
                def.getDataFile().c_str());
        spSkeletonJson_dispose(json);
        spAtlas_dispose(atlas);
        if(data == nullptr)
        {
            throw Exception(json->error ? json->error :
                "Error reading skeleton data.");
        }
        return data;
    }

    void SpineManager::deleteSkeletonData(spSkeletonData* data)
    {
        spSkeletonData_dispose(data);
    }

    SpineSkeleton SpineManager::load(const std::string& name)
    {
        auto itr = _datas.find(name);
        if(itr == _datas.end())
        {
            auto& def = _definitions.get(name);
            auto data = std::shared_ptr<spSkeletonData>(
                loadData(def), &deleteSkeletonData);
            _datas.insert(itr, {name, data});
        }
        return SpineSkeleton(itr->second);
    }

}
