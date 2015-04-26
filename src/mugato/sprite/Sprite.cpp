
#include <mugato/sprite/Sprite.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <gorn/render/RenderKinds.hpp>
#include <mugato/base/Exception.hpp>

namespace mugato {

    const char* Sprite::kDefaultAnimation = "default";
    const char* Sprite::kDefaultFrame = "default";

    Sprite::Sprite()
    {
    }

    Sprite::Sprite(const Animation& anim):
    _anims({{kDefaultAnimation, anim}})
    {
    }

    Sprite::Sprite(const std::shared_ptr<gorn::Material>& material):
    _anims({{kDefaultAnimation, Animation(material)}})
    {
    }

    Sprite::Sprite(const std::shared_ptr<gorn::Material>& material,
        const Region& region):
    _anims({{kDefaultAnimation, Animation(material, region)}})
    {
    }

    glm::vec2 Sprite::Sprite::getSize() const
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
            itr->second.update(dt);
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

