

#ifndef __mugato__SpriteFrame__
#define __mugato__SpriteFrame__

#include <memory>
#include <glm/glm.hpp>
#include <buffer.hpp>
#include <mugato/sprite/SpriteAtlasRegion.hpp>

namespace gorn {
    class RenderQueue;
    class Material;
}

namespace mugato {

    class SpriteFrame
    {
    public:
        typedef SpriteAtlasRegion Region;
    private:
        std::shared_ptr<gorn::Material> _material;
        Region _region;

        bool _dirtyTexVerts;
        bool _dirtyPosVerts;

        buffer _texVerts;
        buffer _posVerts;

        void updatePositionData();
        void updateTextureData();

        void init();

    public:
        SpriteFrame();
        explicit SpriteFrame(const std::shared_ptr<gorn::Material>& material);
        SpriteFrame(const std::shared_ptr<gorn::Material>& material, const Region& region);

        void setMaterial(const std::shared_ptr<gorn::Material>& material);
        const std::shared_ptr<gorn::Material>& getMaterial() const;

        void setRegion(const Region& region);
        const Region& getRegion() const;

        const glm::vec2 getSize() const;

        void update();
        void render(gorn::RenderQueue& queue) const;
    };

}

#endif
