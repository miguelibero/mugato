#ifndef __mugato__SpriteComponent__
#define __mugato__SpriteComponent__

#include <mugato/scene/Component.hpp>
#include <mugato/sprite/Sprite.hpp>
#include <mugato/sprite/SpriteEnums.hpp>
#include <string>

namespace mugato
{
    class SpriteComponent : public Component
    {
    public:
        typedef SpriteResizeMode ResizeMode;
    private:
        std::string _name;
        std::string _anim;
        ResizeMode _resizeMode;
        Sprite _sprite;
        bool _resizeEntity;
    public:
        SpriteComponent(const std::string& name);
        SpriteComponent(const std::string& name, ResizeMode mode);

        void setResizeEntity(bool enabled);
        void setAnimation(const std::string& name);
        Sprite& getSprite();
        const Sprite& getSprite() const;

        void onAssignedToContext(Context& ctx) override;
        void onAddedToEntity(Entity& entity) override;
        void onEntityTransformChanged(Entity& entity) override;
        void update(double dt) override;
        void render(gorn::RenderQueue& queue) override;
    };
}

#endif
