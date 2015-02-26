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
    public:
        SpriteComponent(const std::string& name);

        void setAnimation(const std::string& name);
        Sprite& getSprite();
        const Sprite& getSprite() const;

        void onAddedToEntity(const Entity& entity) override;
        void update(double dt) override;
        void render(gorn::RenderQueue& queue) override;
    };
}

#endif
