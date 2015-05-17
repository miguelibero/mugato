

#ifndef __mugato__SpriteFrameLabelCharacter__
#define __mugato__SpriteFrameLabelCharacter__

#include <mugato/sprite/SpriteFrame.hpp>
#include <mugato/label/FontAtlasRegion.hpp>
#include <mugato/label/LabelCharacter.hpp>

namespace mugato {

    class LabelCharacter
    {
    public:
        typedef FontAtlasRegion Region;
    private:
        SpriteFrame _frame;
        Region _region;
        glm::mat4 _transform;
        bool _dirty;

    public:
        LabelCharacter();
        explicit LabelCharacter(
            const std::shared_ptr<gorn::Material>& material);
        LabelCharacter(const std::shared_ptr<gorn::Material>& material,
            const Region& region);

        void setRegion(const Region& region);
        const Region& getRegion() const;

        void update();
        void render(gorn::RenderQueue& queue) const;

    };

}

#endif
