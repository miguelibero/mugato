#ifndef __mugato__Component__
#define __mugato__Component__

#include <glm/glm.hpp>
#include <mugato/scene/EntityEnums.hpp>

namespace gorn
{
    class RenderQueue;
}

namespace mugato
{
    class Entity;

    class Component
    {
    public:
        typedef EntityTouchPhase TouchPhase;

        virtual ~Component(){};
        virtual void onAddedToEntity(Entity& entity){};
        virtual void onEntityTransformChanged(Entity& entity){};
        virtual bool onEntityTouched(Entity& entity,
            const glm::vec2& p, TouchPhase phase) { return false; };
        virtual bool hasFinished() const { return false; };        
        virtual void update(double dt){};
        virtual void fixedUpdate(double dt){};
        virtual void render(gorn::RenderQueue& queue){};
    };
}

#endif
