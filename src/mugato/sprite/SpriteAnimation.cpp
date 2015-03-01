
#include <mugato/sprite/SpriteAnimation.hpp>
#include <gorn/render/Material.hpp>

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

    glm::vec2 SpriteAnimation::getSize() const
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
        getCurrentFrame().update();
    }

    void SpriteAnimation::render(gorn::RenderQueue& queue) const
    {
        getCurrentFrame().render(queue);
    }

}


