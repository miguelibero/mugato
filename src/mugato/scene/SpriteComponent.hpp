#ifndef __mugato__SpriteComponent__
#define __mugato__SpriteComponent__

#include <mugato/scene/Component.hpp>
#include <mugato/sprite/Sprite.hpp>
#include <string>

namespace mugato
{
    class SpriteComponent : public Component
    {
    private:
        std::string _name;
        std::string _animation;
        Sprite _sprite;
        std::weak_ptr<Entity> _entity;
        glm::vec2 _pivotPercent;
    public:
        SpriteComponent(const std::string& name);

        void setAnimation(const std::string& name);
        Sprite& getSprite();
        const Sprite& getSprite() const;

        void setPivotPercent(const glm::vec2& val);

        void onAddedToEntity(Entity& entity) override;
        void update(double dt) override;
        void render(gorn::RenderQueue& queue) override;
    };
}

#endif
