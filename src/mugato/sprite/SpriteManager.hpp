

#ifndef __mugato__SpriteManager__
#define __mugato__SpriteManager__

#include <gorn/asset/AssetManager.hpp>
#include <gorn/base/DefinitionManager.hpp>
#include <mugato/sprite/SpriteDefinition.hpp>

namespace gorn {
    class FileManager;
    class MaterialManager;
}

namespace mugato {

    class Sprite;
    class SpriteAtlas;
    class SpriteFrame;

    class SpriteManager
    {
    public:
        typedef SpriteAtlas Atlas;
        typedef SpriteDefinition Definition;
        typedef std::vector<std::shared_ptr<SpriteFrame>> FrameList;
        typedef std::map<std::string, FrameList> FrameMap;
        typedef gorn::AssetManager<Atlas> Atlases;
        typedef gorn::DefinitionManager<Definition> Definitions;
    private:
        gorn::MaterialManager& _materials;
        Atlases _atlases;
        std::map<std::string, FrameMap> _frames;
        Definitions _definitions;
    public:
        SpriteManager(gorn::MaterialManager& materials, gorn::FileManager& files);

        const Atlases& getAtlases() const;
        Atlases& getAtlases();

        const Definitions& getDefinitions() const;
        Definitions& getDefinitions();

        Sprite load(const std::string& name);
        FrameList loadFrames(
            const std::string& aname, const std::string& fname);
        const FrameMap& loadFrames(
            const std::string& aname);

    };
}

#endif
