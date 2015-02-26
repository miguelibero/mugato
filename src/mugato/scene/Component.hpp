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
        virtual void onAddedToEntity(const Entity& entity){};
        virtual void onRemovedFromEntity(const Entity& entity){};        
        virtual void update(double dt) = 0;
        virtual void render(gorn::RenderQueue& queue) = 0;
    };
}

#endif
