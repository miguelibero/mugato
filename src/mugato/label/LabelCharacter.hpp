

#ifndef __mugato__SpriteFrameLabelCharacter__
#define __mugato__SpriteFrameLabelCharacter__

#include <mugato/sprite/SpriteFrame.hpp>
#include <mugato/label/FontAtlasRegion.hpp>
#include <mugato/label/LabelCharacter.hpp>

namespace mugato {

    enum class LabelCharacterMode
    {
        Character,
        Line
    };

    class LabelCharacter
    {
    public:
        typedef FontAtlasRegion Region;
        typedef LabelCharacterMode Mode;
    private:
        SpriteFrame _frame;
        Region _region;
        glm::mat4 _transform;
        glm::vec2 _base;
        Mode _mode;
        bool _dirty;

    public:
        LabelCharacter();
        LabelCharacter(const glm::vec2& base,
            Mode mode=Mode::Character);
        explicit LabelCharacter(
            const std::shared_ptr<gorn::Material>& material);
        LabelCharacter(const std::shared_ptr<gorn::Material>& material,
            const Region& region);

        Mode getMode() const;
        const glm::vec2& getBase() const;

        void setRegion(const Region& region);
        const Region& getRegion() const;

        void update();
        void render(gorn::RenderQueue& queue) const;

    };

}

#endif
