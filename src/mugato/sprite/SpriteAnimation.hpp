

#ifndef __mugato__SpriteAnimation__
#define __mugato__SpriteAnimation__

#include <mugato/sprite/SpriteFrame.hpp>
#include <memory>

namespace gorn {
    class RenderQueue;
    class Material;
}

namespace mugato {


    class SpriteAnimation
    {
    public:
        typedef SpriteFrame Frame;
        typedef SpriteAtlasRegion Region;
    private:
        std::vector<std::shared_ptr<Frame>> _frames;
        double _frameDuration;
        double _time;

        void init();
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

        size_t getCurrentFrameNumber() const;
        const Frame& getCurrentFrame() const;
        Frame& getCurrentFrame();
        
        double getCurrentTime() const;
        double getDuration() const;

        glm::vec2 getSize() const;

        void update(double dt);
        void render(gorn::RenderQueue& queue) const;
    };

}

#endif
