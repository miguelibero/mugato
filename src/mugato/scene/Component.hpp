#ifndef __mugato__Component__
#define __mugato__Component__

#include <glm/glm.hpp>
#include <mugato/scene/EntityEnums.hpp>
#include <memory>

namespace gorn
{
    class RenderQueue;
	class RenderCamera;
}

namespace mugato
{
    class Context;
    class Entity;

    class Component
    {
    public:
        typedef EntityTouchPhase TouchPhase;
		typedef size_t type_t;

        virtual ~Component(){};
        virtual void onAssignedToContext(Context& ctx){};
        virtual void onAddedToEntity(Entity& entity){};
        virtual void onEntityTransformChanged(Entity& entity){};
		virtual void onEntityParentTransformChanged(Entity& entity, Entity& parent){};
        virtual bool onScreenTouched(Entity& entity, const gorn::RenderCamera& cam,
			const glm::vec2& p, TouchPhase phase) { return false; };
		virtual void onEntityLayersChanged(Entity& entity) {};
		virtual void onEntityComponentsLoaded(Entity& entity){};
		virtual void beforeEntityChildrenRender(gorn::RenderQueue& queue) {};
		virtual void afterEntityChildrenRender(gorn::RenderQueue& queue) {};
        virtual void onEntityChildTransformChanged(Entity& entity,
            Entity& child) {};
        virtual void onEntityChildAdded(Entity& entity,
            Entity& child) {};
        virtual void onEntityChildRemoved(Entity& entity,
            Entity& child) {};
        virtual void update(double dt){};
        virtual void fixedUpdate(double dt){};
        virtual void render(gorn::RenderQueue& queue){};
    };

	template<typename T>
	struct ComponentType
	{
		static void id()
		{
		}
	};

	template<typename T>
	size_t ComponentTypeId()
	{
		return reinterpret_cast<size_t>(&ComponentType<T>::id);
	}
}

#endif
