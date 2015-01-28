

#ifndef __mugato__LabelCharacter__
#define __mugato__LabelCharacter__

#include <mugato/sprite/SpriteFrame.hpp>
#include <mugato/label/LabelAtlasRegion.hpp>

namespace mugato {

    class LabelCharacter : public SpriteFrame
    {
    public:
        typedef LabelAtlasRegion Region;
    private:
        bool _dirty;
        glm::mat4 _transform;
        Region _region;
        SpriteFrame _frame;

        void init();

    public:
        LabelCharacter();
        explicit LabelCharacter(const std::shared_ptr<gorn::Material>& material);
        LabelCharacter(const std::shared_ptr<gorn::Material>& material, const Region& region);

        void setRegion(const Region& region);
        const Region& getRegion() const;

        void update();
        void render(gorn::RenderQueue& queue) const;

    };

}

#endif
