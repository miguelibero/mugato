
#include <mugato/scene/SpriteComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/scene/EntityTransform.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/sprite/SpriteManager.hpp>

namespace mugato
{
    SpriteComponent::SpriteComponent(const std::string& name):
    _name(name), _resizeEntity(false)
    {
    }

    SpriteComponent::SpriteComponent(const std::string& name, ResizeMode mode):
    _name(name), _resizeEntity(false)
    {
        _sprite.setResizeMode(mode);
    }

    void SpriteComponent::setResizeEntity(bool enabled)
    {
        _resizeEntity = enabled;
    }

    Sprite& SpriteComponent::getSprite()
    {
        return _sprite;
    }

    const Sprite& SpriteComponent::getSprite() const
    {
        return _sprite;
    }

    void SpriteComponent::setAnimation(const std::string& name)
    {
        _anim = name;
        if(!_anim.empty())
        {
            _sprite.play(_anim);
        }
    }

    void SpriteComponent::onAddedToEntity(Entity& entity)
    {
        auto resizeMode = _sprite.getResizeMode();
        _sprite = entity.getContext().getSprites().load(_name);
        _sprite.setResizeMode(resizeMode);
        if(!_anim.empty())
        {
            _sprite.play(_anim);
        }
        onEntityTransformChanged(entity);
    }

    void SpriteComponent::onEntityTransformChanged(Entity& entity)
    {
        auto& size = entity.getTransform().getSize();
        _sprite.setSize(glm::vec2(size));
        if(_resizeEntity)
        {
            entity.getTransform().setSize(_sprite.getContentSize());
        }
    }

    void SpriteComponent::update(double dt)
    {
        _sprite.update(dt);
    }

    void SpriteComponent::render(gorn::RenderQueue& queue)
    {
        _sprite.render(queue);
    }

}
