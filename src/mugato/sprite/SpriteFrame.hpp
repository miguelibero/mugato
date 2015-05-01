

#ifndef __mugato__SpriteFrame__
#define __mugato__SpriteFrame__

#include <memory>
#include <glm/glm.hpp>
#include <buffer.hpp>
#include <mugato/sprite/SpriteAtlasRegion.hpp>
#include <mugato/sprite/SpriteEnums.hpp>

namespace gorn {
    class RenderQueue;
    class Material;
}

namespace mugato {

    class SpriteFrame
    {
    public:
        typedef SpriteResizeMode ResizeMode;
        typedef SpriteAtlasRegion Region;
    private:
        std::shared_ptr<gorn::Material> _material;
        Region _region;

        bool _dirtyTexVerts;
        bool _dirtyPosVerts;

        buffer _texVerts;
        buffer _posVerts;

        mutable glm::vec2 _size;
        ResizeMode _resizeMode;
        mutable bool _dirtySize;

        glm::vec4 _stretchBorders;

        void updatePositionData();
        void updateTextureData();
        void updateSize() const;

        void init();

    public:
        SpriteFrame();
        explicit SpriteFrame(const std::shared_ptr<gorn::Material>& material);
        SpriteFrame(const std::shared_ptr<gorn::Material>& material, const Region& region);

        void setMaterial(const std::shared_ptr<gorn::Material>& material);
        const std::shared_ptr<gorn::Material>& getMaterial() const;

        void setRegion(const Region& region);
        const Region& getRegion() const;

        const glm::vec2& getSize() const;
        void setSize(const glm::vec2& size);
        void setResizeMode(ResizeMode mode);

        bool hasStretchBorders() const;
        void setStretchBorders(const glm::vec4& borders);

        void update();
        void render(gorn::RenderQueue& queue) const;
    };

}

#endif
