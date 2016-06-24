

#ifndef __mugato__SpriteAnimation__
#define __mugato__SpriteAnimation__

#include <mugato/sprite/SpriteFrame.hpp>
#include <mugato/sprite/SpriteEnums.hpp>
#include <memory>

namespace gorn
{
    class RenderCommand;
    class Material;
}

namespace mugato {

    class SpriteAnimation
    {
    public:
        typedef SpriteResizeMode ResizeMode;
        typedef SpriteFrame Frame;
        typedef SpriteAtlasRegion Region;
		typedef std::vector<Frame> FrameList;
    private:
		FrameList _frames;
        double _frameDuration;
        double _time;
        glm::vec4 _stretchBorders;
        mutable glm::vec2 _size;
        ResizeMode _resizeMode;

        void init();
    public:
        SpriteAnimation();
        SpriteAnimation(const std::shared_ptr<gorn::Material>& material);
        SpriteAnimation(const std::shared_ptr<gorn::Material>& material,
            const Region& region);
        SpriteAnimation(const Frame& frame);
        
        SpriteAnimation& withFrameDuration(double duration);
        SpriteAnimation& withFrames(const FrameList& frames);
        SpriteAnimation& addFrame(const Frame& frame);
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
        gorn::RenderCommand render() const;
    };

}

#endif
