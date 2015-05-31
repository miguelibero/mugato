

#ifndef __mugato__SpriteAnimation__
#define __mugato__SpriteAnimation__

#include <mugato/sprite/SpriteFrame.hpp>
#include <mugato/sprite/SpriteEnums.hpp>
#include <memory>

namespace gorn {
    class RenderQueue;
    class Material;
}

namespace mugato {


    class SpriteAnimation
    {
    public:
        typedef SpriteResizeMode ResizeMode;
        typedef SpriteFrame Frame;
        typedef SpriteAtlasRegion Region;
    private:
        std::vector<std::shared_ptr<Frame>> _frames;
        double _frameDuration;
        double _time;
        glm::vec4 _stretchBorders;
        mutable glm::vec2 _size;
        ResizeMode _resizeMode;

        void init();
        void updateSize();
    public:
        SpriteAnimation();
        SpriteAnimation(const std::shared_ptr<gorn::Material>& material);
        SpriteAnimation(const std::shared_ptr<gorn::Material>& material,
            const Region& region);
        SpriteAnimation(const std::shared_ptr<Frame>&);
        
        SpriteAnimation& withFrameDuration(double duration);
        SpriteAnimation& withFrames(const std::vector<std::shared_ptr<Frame>>& frames);
        SpriteAnimation& addFrame(const std::shared_ptr<Frame>& frame);
        SpriteAnimation& addFrame(const std::shared_ptr<gorn::Material>& material);
        SpriteAnimation& addFrame(const std::shared_ptr<gorn::Material>& material,
            const Region& region);

        void setMaterial(const std::shared_ptr<gorn::Material>& material);
        std::shared_ptr<gorn::Material> getMaterial() const;

        size_t getCurrentFrameNumber() const;
        const Frame& getCurrentFrame() const;
        Frame& getCurrentFrame();
        
        double getCurrentTime() const;
        double getDuration() const;

        const glm::vec2& getSize() const;
        glm::vec2 getContentSize() const;
        void setSize(const glm::vec2& size);
        ResizeMode getResizeMode() const;
        void setResizeMode(ResizeMode mode);
        const glm::vec4& getStretchBorders() const;
        void setStretchBorders(const glm::vec4& borders);

        void update(double dt);
        void render(gorn::RenderQueue& queue) const;
    };

}

#endif
