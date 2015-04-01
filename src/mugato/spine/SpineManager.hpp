#ifndef __mugato__SpineManager__
#define __mugato__SpineManager__

#include <mugato/spine/SpineSkeletonData.hpp>
#include <mugato/spine/SpineAtlas.hpp>
#include <mugato/spine/SpineSkeletonDefinition.hpp>
#include <gorn/base/DefinitionManager.hpp>
#include <map>
#include <memory>

namespace gorn {
    class FileManager;
    class MaterialManager;
}

struct spSkeletonData;

namespace mugato {

    class SpineSkeleton;

    class SpineManager
    {
    public:
        typedef SpineSkeletonDefinition Definition;
        typedef gorn::DefinitionManager<Definition> Definitions;
        typedef SpineAtlas Atlas;
        typedef SpineSkeletonData SkeletonData;
    private:
        typedef std::map<std::string, std::shared_ptr<SkeletonData>> DataMap;
        typedef std::map<std::string, std::shared_ptr<Atlas>> AtlasMap;
        Definitions _definitions;
        gorn::MaterialManager& _materials;
        gorn::FileManager& _files;
        DataMap _datas;
        AtlasMap _atlases;

        std::shared_ptr<SkeletonData> loadData(const Definition& def);
    public:
        SpineManager(gorn::MaterialManager& materials, gorn::FileManager& files);
        ~SpineManager();

        const Definitions& getDefinitions() const;
        Definitions& getDefinitions();

        gorn::MaterialManager& getMaterials();
        gorn::FileManager& getFiles();

        SpineSkeleton load(const std::string& name);
    };
}

#endif
