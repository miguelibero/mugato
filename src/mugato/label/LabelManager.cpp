
#include <mugato/label/LabelManager.hpp>
#include <mugato/label/Label.hpp>
#include <mugato/label/FontAtlas.hpp>

namespace mugato {
    
    LabelManager::LabelManager(gorn::MaterialManager& materials,
        gorn::FileManager& files):
    _materials(materials), _atlases(files)
    {
        getDefinitions().set([](const std::string& name){
            return Definition()
                .withAtlas(name);
        });
    }

    const LabelManager::Atlases& LabelManager::getAtlases() const
    {
        return _atlases;
    }

    LabelManager::Atlases& LabelManager::getAtlases()
    {
        return _atlases;
    }

    const LabelManager::Definitions& LabelManager::getDefinitions() const
    {
        return _definitions;
    }

    LabelManager::Definitions& LabelManager::getDefinitions()
    {
        return _definitions;
    }

    std::shared_ptr<LabelManager::Font> LabelManager::loadFont(const std::string& name)
    {
        auto itr = _fonts.find(name);
        if(itr == _fonts.end())
        {
            auto atlas = _atlases.load(name).get();
            auto font = std::make_shared<Font>(atlas->createFont(_materials));
            itr = _fonts.insert(itr, {name, font});
        }
        return itr->second;
    }

    Label LabelManager::load(const std::string& name)
    {
        auto& def = getDefinitions().get(name);
        return Label(loadFont(def.getAtlas()));
    }
}

