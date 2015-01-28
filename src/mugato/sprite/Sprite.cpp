
#include <mugato/sprite/Sprite.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <gorn/render/Kinds.hpp>
#include <mugato/base/Exception.hpp>

namespace mugato {

    const char* Sprite::kDefaultAnimation = "default";

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

