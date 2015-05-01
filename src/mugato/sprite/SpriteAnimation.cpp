
#include <mugato/sprite/SpriteAnimation.hpp>
#include <gorn/gl/Material.hpp>

namespace mugato {

    SpriteAnimation::SpriteAnimation()
    {
        init();
    }

    SpriteAnimation::SpriteAnimation(
        const std::shared_ptr<gorn::Material>& material):
    _frames{std::make_shared<Frame>(material)}
    {
        init();
    }

    SpriteAnimation::SpriteAnimation(
        const std::shared_ptr<gorn::Material>& material,
        const Region& region):
    _frames{std::make_shared<Frame>(material, region)}
    {
        init();
    }

    SpriteAnimation::SpriteAnimation(const std::shared_ptr<Frame>& frame):
    _frames{frame}
    {
        init();
    }
    
    void SpriteAnimation::init()
    {
        _time = 0.0;
        _frameDuration = 1.0/30.0;
        _resizeMode = ResizeMode::Original;
    }

    SpriteAnimation& SpriteAnimation::withFrameDuration(double duration)
    {
        _frameDuration = duration;
        return *this;
    }

    SpriteAnimation& SpriteAnimation::withFrames(
        const std::vector<std::shared_ptr<Frame>>& frames)
    {
        _frames = frames;
        return *this;
    }

    SpriteAnimation& SpriteAnimation::addFrame(
        const std::shared_ptr<Frame>& frame)
    {
        _frames.push_back(frame);
        return *this;
    }

    SpriteAnimation& SpriteAnimation::addFrame(
        const std::shared_ptr<gorn::Material>& material)
    {
        _frames.push_back(std::make_shared<Frame>(material));
        return *this;
    }

    SpriteAnimation& SpriteAnimation::addFrame(
        const std::shared_ptr<gorn::Material>& material,
        const Region& region)
    {
        _frames.push_back(std::make_shared<Frame>(material, region));
        return *this;
    }

    size_t SpriteAnimation::getCurrentFrameNumber() const
    {
        return ((size_t)(_time/_frameDuration)) % _frames.size();
    }

    const SpriteAnimation::Frame& SpriteAnimation::getCurrentFrame() const
    {
        return *_frames.at(getCurrentFrameNumber());
    }

    SpriteAnimation::Frame& SpriteAnimation::getCurrentFrame()
    {
        return *_frames.at(getCurrentFrameNumber());
    }

    double SpriteAnimation::getCurrentTime() const
    {
        return _time;
    }

    double SpriteAnimation::getDuration() const
    {
        return _frames.size()*_frameDuration;
    }

    const glm::vec2& SpriteAnimation::getSize() const
    {
        return _size;
    }

    glm::vec2 SpriteAnimation::getOriginalSize() const
    {
        glm::vec2 size;
        for(auto& frame : _frames)
        {
            auto& fsize = frame->getSize();
            if(fsize.x > size.x)
            {
                size.x = fsize.x;
            }
            if(fsize.y > size.y)
            {
                size.y = fsize.y;
            }
        }
        return size;
    }

    void SpriteAnimation::setSize(const glm::vec2& size)
    {
        _size = size;
    }

    void SpriteAnimation::setResizeMode(ResizeMode mode)
    {
        _resizeMode = mode;
    }

    void SpriteAnimation::setStretchBorders(const glm::vec4& borders)
    {
        _stretchBorders = borders;
    }

    void SpriteAnimation::update(double dt)
    {
        _time += dt;
        auto duration = getDuration();
        if(duration > 0)
        {
            while(_time > duration)
            {
                _time -= duration;
            }
        }
        auto& frame = getCurrentFrame();
        frame.setSize(_size);
        frame.setResizeMode(_resizeMode);
        frame.setStretchBorders(_stretchBorders);
        frame.update();
    }

    void SpriteAnimation::render(gorn::RenderQueue& queue) const
    {
        getCurrentFrame().render(queue);
    }

}


