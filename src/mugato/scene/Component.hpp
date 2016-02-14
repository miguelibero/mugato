#ifndef __mugato__Component__
#define __mugato__Component__

#include <glm/glm.hpp>
#include <mugato/scene/EntityEnums.hpp>
#include <functional>

namespace gorn
{
    class RenderQueue;
}

namespace mugato
{
    class Context;
    class Entity;

    class Component
    {
    public:
        typedef EntityTouchPhase TouchPhase;

        virtual ~Component(){};
        virtual void onAssignedToContext(Context& ctx){};
        virtual void onAddedToEntity(Entity& entity){};
        virtual void onParentEntitySizeChanged(Entity& entity){};
        virtual void onEntityTransformChanged(Entity& entity){};
        virtual bool onEntityTouched(Entity& entity,
            const glm::vec2& p, TouchPhase phase) { return false; };
        virtual void onEntityComponentsLoaded(Entity& entity){};
		virtual void beforeEntityChildrenRender(gorn::RenderQueue& queue) {};
		virtual void afterEntityChildrenRender(gorn::RenderQueue& queue) {};
        virtual void update(double dt){};
        virtual void fixedUpdate(double dt){};
        virtual void render(gorn::RenderQueue& queue){};
    };
}

#endif
