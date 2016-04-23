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
        std::weak_ptr<Entity> _entity;
    public:

        EntityStack();

        bool touch(const gorn::RenderCamera& cam,
			const glm::vec2& p, TouchPhase phase =TouchPhase::Begin);

        void onAssignedToContext(Context& ctx) override;
        void onEntityTransformChanged(Entity& entity) override;
		bool onScreenTouched(Entity& entity, const gorn::RenderCamera& cam,
			const glm::vec2& p, TouchPhase phase) override;
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
