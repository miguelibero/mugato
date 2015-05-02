
#include <mugato/sprite/Sprite.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <gorn/render/RenderKinds.hpp>
#include <mugato/base/Exception.hpp>

namespace mugato {

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
        _anims[kDefaultAnimation] = Animation(material);
    }

    const glm::vec2& Sprite::getSize() const
    {
        return _size;
    }

    glm::vec2 Sprite::getOriginalSize() const
    {
        glm::vec2 size;
        for(auto itr = _anims.begin(); itr != _anims.end(); ++itr)
        {
            auto asize = itr->second.getSize();
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

    void Sprite::setAnimation(const std::string& name, const Animation& anim)
    {
        _anims[name] = anim;
    }

    Sprite::Animation& Sprite::setAnimation(const std::string& name)
    {
        _anims[name] = Animation();
        return _anims[name];
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

    void Sprite::render(gorn::RenderQueue& queue) const
    {
        auto itr = _anims.find(_currentAnim);
        if(itr != _anims.end())
        {
            itr->second.render(queue);
        }
    
    }

}

