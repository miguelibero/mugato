

#ifndef __mugato__LabelManager__
#define __mugato__LabelManager__

#include <mugato/label/LabelDefinition.hpp>
#include <mugato/label/LabelFont.hpp>
#include <gorn/asset/AssetManager.hpp>
#include <gorn/gl/MaterialManager.hpp>
#include <gorn/base/DefinitionManager.hpp>
#include <map>

namespace gorn {
    class MaterialManager;
    class FileManager;
}

namespace mugato {

    class Label;
    class FontAtlas;

    class LabelManager
    {
    public:
        typedef FontAtlas Atlas;
        typedef LabelFont Font;
        typedef LabelDefinition Definition;
        typedef gorn::AssetManager<Atlas> Atlases;
        typedef gorn::DefinitionManager<Definition> Definitions;
        typedef std::map<std::string, std::shared_ptr<Font>> Fonts;
    private:
        gorn::MaterialManager& _materials;
        Atlases _atlases;
        Fonts _fonts;
        Definitions _definitions;
    public:
        LabelManager(gorn::MaterialManager& materials, gorn::FileManager& files);
        LabelManager(const LabelManager& other) = delete;

        const Atlases& getAtlases() const;
        Atlases& getAtlases();

        const Definitions& getDefinitions() const;
        Definitions& getDefinitions();

        std::shared_ptr<Font> loadFont(const std::string& name);
        std::shared_ptr<Font> loadLabelFont(const std::string& name);
        Label load(const std::string& name);
        Label tryLoad(const std::string& name);
    };

}

#endif
