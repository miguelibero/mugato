
#include <mugato/sprite/SpriteDefinition.hpp>

namespace mugato
{

    SpriteDefinition::SpriteDefinition():
	_resizeMode(ResizeMode::Original)
    {
    }

    SpriteDefinition& SpriteDefinition::withAtlas(const std::string& name)
    {
        _atlas = name;
        return *this;
    }

    SpriteDefinition& SpriteDefinition::withMaterial(const std::string& name)
    {
        _material = name;
        return *this;
    }

    SpriteDefinition& SpriteDefinition::withStretchBorders(const glm::vec4& borders)
    {
        _stretchBorders = borders;
        return *this;
    }

    SpriteDefinition& SpriteDefinition::withAnimation(const std::string& name,
        const SpriteDefinition::Animation& anim)
    {
        _anims[name] = anim;
        return *this;
    }

    SpriteDefinition& SpriteDefinition::withResizeMode(ResizeMode mode)
    {
        _resizeMode = mode;
        return *this;
    }

    SpriteDefinition::Animation& SpriteDefinition::setAnimation(
        const std::string& name)
    {
        auto itr = _anims.find(name);
        if(itr == _anims.end())
        {
            itr = _anims.insert(itr, {name, Animation()});
        }
        return itr->second;
    }

    const std::string& SpriteDefinition::getAtlas() const
    {
        return _atlas;
    }

    const std::string& SpriteDefinition::getMaterial() const
    {
        return _material;
    }

    const SpriteDefinition::Animation& SpriteDefinition::getAnimation(
        const std::string& name) const
    {
        return _anims.at(name);
    }

    bool SpriteDefinition::hasAnimation(const std::string& name) const
    {
        return _anims.find(name) != _anims.end();
    }

    const SpriteDefinition::AnimationMap& SpriteDefinition::getAnimations() const
    {
        return _anims;
    }

    const glm::vec4& SpriteDefinition::getStretchBorders() const
    {
        return _stretchBorders;
    }

    SpriteDefinition::ResizeMode SpriteDefinition::getResizeMode() const
    {
        return _resizeMode;
    }

}
