#ifndef __mugato__EntityStack__
#define __mugato__EntityStack__

#include <mugato/scene/Component.hpp>
#include <mugato/scene/EntityTransform.hpp>
#include <deque>
#include <memory>

namespace mugato
{
    class Context;

    class EntityStack : public Component
    {
    public:
        typedef std::deque<std::shared_ptr<Entity>> Stack;

    private:
        Stack _stack;
        Context* _ctx;
        EntityTransform _transform;
    public:

        EntityStack();

        void setContext(Context& ctx);

        const EntityTransform& getTransform() const;
        EntityTransform& getTransform();

        void touch(const glm::vec2& p);

        void onEntityTransformChanged(Entity& entity) override;
        void onEntityTouched(Entity& entity, const glm::vec2& p) override;
        void onAddedToEntity(Entity& entity) override;
        void update(double dt) override;
        void fixedUpdate(double dt) override;
        void render(gorn::RenderQueue& queue) override;
        
        std::shared_ptr<Entity> push(std::shared_ptr<Entity> entity=nullptr);
        std::shared_ptr<Entity> get();
        void pop();
        std::shared_ptr<Entity> set(std::shared_ptr<Entity> entity=nullptr);
    };
}

#endif
