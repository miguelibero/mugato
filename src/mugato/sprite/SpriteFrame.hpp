

#ifndef __mugato__SpriteFrame__
#define __mugato__SpriteFrame__

#include <memory>
#include <glm/glm.hpp>
#include <gorn/base/Data.hpp>
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
        bool _dirtyElmVerts;

        mutable gorn::Data _texVerts;
        mutable gorn::Data _posVerts;
        mutable gorn::Data _elmVerts;

        void updateElementData();
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
