#ifndef __mugato__Component__
#define __mugato__Component__

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
        virtual ~Component(){};
        virtual void onAddedToEntity(Entity& entity){};
        virtual void onEntityTransformChanged(Entity& entity){};
        virtual bool hasFinished() const { return false; };        
        virtual void update(double dt){};
        virtual void fixedUpdate(double dt){};
        virtual void render(gorn::RenderQueue& queue){};
    };
}

#endif
