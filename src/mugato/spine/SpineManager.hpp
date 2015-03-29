#ifndef __mugato__SpineManager__
#define __mugato__SpineManager__

#include <spine/SkeletonData.h>
#include <spine/Atlas.h>
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
    private:
        typedef std::map<std::string, std::shared_ptr<spSkeletonData>> DataMap;
        typedef std::map<std::string, std::shared_ptr<spAtlas>> AtlasMap;
        Definitions _definitions;
        gorn::MaterialManager& _materials;
        gorn::FileManager& _files;
        DataMap _datas;
        AtlasMap _atlases;

        static void deleteSkeletonData(spSkeletonData* data);
        spSkeletonData* loadData(const Definition& def);
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
