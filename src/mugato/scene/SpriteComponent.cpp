
#include <mugato/scene/SpriteComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/sprite/SpriteManager.hpp>

namespace mugato
{
    SpriteComponent::SpriteComponent(const std::string& name):
    _name(name), _animation(Sprite::kDefaultAnimation)
    {
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
        _sprite.play(_animation);
        _animation = name;
    }

    void SpriteComponent::onAddedToEntity(const Entity& entity)
    {
        _sprite = entity.getContext().getSprites().load(_name);
        _sprite.play(_animation);
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
