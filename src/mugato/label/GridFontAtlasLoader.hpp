
#ifndef __mugato__GridFontAtlasLoader__

#include <gorn/asset/AssetLoader.hpp>
#include <glm/glm.hpp>

namespace gorn {
    class MaterialManager;
}

namespace mugato {

    class FontAtlas;

    class GridFontAtlasLoader : public gorn::AssetLoader<FontAtlas>
    {
    private:
        gorn::MaterialManager& _materials;
        glm::vec2 _regionSize;
        char _initialCharacter;
        float _advanceDifference;
    public:
        GridFontAtlasLoader(gorn::MaterialManager& textures, const glm::vec2& regionSize);
        bool validate(const std::string& name) const;
        FontAtlas load(const std::string& name) const;
        void setInitialCharacter(char chr);
        void setAdvanceDifference(float val);
    };
}

#endif
