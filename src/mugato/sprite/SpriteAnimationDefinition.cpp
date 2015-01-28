
#include <mugato/sprite/SpriteAnimationDefinition.hpp>

namespace mugato {

    SpriteAnimationDefinition::SpriteAnimationDefinition():
    _frameDuration(0.0f)
    {
    }

    SpriteAnimationDefinition& SpriteAnimationDefinition::withAtlas(
        const std::string& atlas)
    {
        _atlas = atlas;
        return *this;
    }

    SpriteAnimationDefinition& SpriteAnimationDefinition::withFrames(
        const std::string& frame)
    {
        _frames.push_back(frame);
        return *this;
    }

    SpriteAnimationDefinition& SpriteAnimationDefinition::withFrameDuration(
        double duration)
    {
        _frameDuration = duration;
        return *this;
    }

    const std::vector<std::string>& SpriteAnimationDefinition::getFrames() const
    {
        return _frames;
    }

    double SpriteAnimationDefinition::getFrameDuration() const
    {
        return _frameDuration;
    }

    const std::string& SpriteAnimationDefinition::getAtlas() const
    {
        return _atlas;
    }

}

