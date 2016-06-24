
#include <mugato/sprite/Sprite.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <gorn/render/RenderKinds.hpp>
#include <mugato/base/Exception.hpp>

namespace mugato
{

    const char* Sprite::kDefaultAnimation = "default";
    const char* Sprite::kDefaultFrame = "default";

    Sprite::Sprite()
    {
        init();
    }

    Sprite::Sprite(const Animation& anim):
    _anims({{kDefaultAnimation, anim}})
    {
        init();
    }

    Sprite::Sprite(const std::shared_ptr<gorn::Material>& material):
    _anims({{kDefaultAnimation, Animation(material)}})
    {
        init();
    }

    Sprite::Sprite(const std::shared_ptr<gorn::Material>& material,
        const Region& region):
    _anims({{kDefaultAnimation, Animation(material, region)}})
    {
        init();
    }

    void Sprite::init()
    {
        _resizeMode = ResizeMode::Original;
        _currentAnim = kDefaultAnimation;
    }

    void Sprite::setMaterial(const std::shared_ptr<gorn::Material>& material)
    {
        auto itr = _anims.find(_currentAnim);
        if(itr == _anims.end())
        {
            _anims.insert(itr, {_currentAnim, Animation(material)});
        }
        else
        {
            itr->second.setMaterial(material);
        }
    }

    std::shared_ptr<gorn::Material> Sprite::getMaterial() const
    {
        return getCurrentAnimation().getMaterial();
    }

    const glm::vec2& Sprite::getSize() const
    {
        return _size;
    }

    glm::vec2 Sprite::getContentSize() const
    {
        glm::vec2 size;
        for(auto itr = _anims.begin(); itr != _anims.end(); ++itr)
        {
            auto asize = itr->second.getContentSize();
            if(asize.x > size.x)
            {
                size.x = asize.x;
            }
            if(asize.y > size.y)
            {
                size.y = asize.y;
            }
        }
        return size;
    }

    void Sprite::setSize(const glm::vec2& size)
    {
        _size = size;
    }

    void Sprite::setResizeMode(ResizeMode mode)
    {
        _resizeMode = mode;
    }

    Sprite::ResizeMode Sprite::getResizeMode() const
    {
        return _resizeMode;
    }

    void Sprite::setStretchBorders(const glm::vec4& borders)
    {
        _stretchBorders = borders;
    }

    const glm::vec4& Sprite::getStretchBorders() const
    {
        return _stretchBorders;
    }

    Sprite::Animation& Sprite::setAnimation(const std::string& name, const Animation& anim)
    {
        _anims[name] = anim;
        return _anims[name];
    }

    Sprite::Animation& Sprite::setAnimation(const std::string& name)
    {
        return setAnimation(name, Animation());
    }

    Sprite::Animation& Sprite::getCurrentAnimation()
    {
        return _anims[_currentAnim];
    }

    const Sprite::Animation& Sprite::getCurrentAnimation() const
    {
        return _anims.at(_currentAnim);
    }

    void Sprite::play(const std::string& name)
    {
        if(_anims.find(name) == _anims.end())
        {
            throw Exception(std::string("Could not find animation '")+name+"'.");
        }
        _currentAnim = name;
    }

    void Sprite::update(double dt)
    {
        auto itr = _anims.find(_currentAnim);
        if(itr != _anims.end())
        {
            auto& anim = itr->second;
            anim.setSize(_size);
            anim.setResizeMode(_resizeMode);
            anim.setStretchBorders(_stretchBorders);
            anim.update(dt);
        }
    }

    gorn::RenderCommand Sprite::render() const
    {
        auto itr = _anims.find(_currentAnim);
        if(itr != _anims.end())
        {
            return itr->second.render();
        }
		return gorn::RenderCommand();
    }

}
