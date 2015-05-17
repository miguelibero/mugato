
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
        float _lineHeight;
    public:
        GridFontAtlasLoader(gorn::MaterialManager& materials,
            const glm::vec2& regionSize, char initialChar=0);
        bool validate(const std::string& name) const NOEXCEPT;
        FontAtlas load(const std::string& name) const;
        void setInitialCharacter(char chr) NOEXCEPT;
        void setAdvanceDifference(float val) NOEXCEPT;
        void setLineHeight(float val) NOEXCEPT;
    };
}

#endif
