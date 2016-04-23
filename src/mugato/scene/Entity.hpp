#ifndef __mugato__Entity__
#define __mugato__Entity__

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <mugato/scene/Component.hpp>
#include <mugato/scene/EntityTransform.hpp>
#include <mugato/scene/EntityEnums.hpp>

namespace gorn
{
    class RenderQueue;
	class RenderCamera;
}

namespace mugato
{
    class Context;
    class Component;
	class Action;

    class Entity final : public std::enable_shared_from_this<Entity>
    {
    public:
        typedef EntityTouchPhase TouchPhase;
        typedef EntityTransform Transform;
        typedef std::vector<std::shared_ptr<Entity>> Children;
        typedef std::vector<std::weak_ptr<Entity>> TouchedChildren;
        typedef std::weak_ptr<Entity> Parent;
        typedef std::vector<std::unique_ptr<Component>> Components;
		typedef std::vector<int> Layers;
    private:
        Context* _ctx;
        Components _componentsToAdd;
        Components _components;
        Children _children;
		Children _removedChildren;
        Parent _parent;
        Transform _transform;
        TouchedChildren _touchedChildren;
		Layers _layers;

		void updateRemovedChildren();
        void updateTransform();
        void addPendingComponents();
        void onChildTransformChanged(Entity& child);
		void onParentTransformChanged(Entity& parent);
    public:

        Entity();
        ~Entity();

        std::shared_ptr<Entity> getSharedPtr();

        void setContext(Context& ctx);
        Context& getContext() const;

        Transform& getTransform();
        const Transform& getTransform() const;
		glm::mat4 getModelMatrix() const;
		glm::mat4 getModelInverse() const;

        bool onScreenTouched(const gorn::RenderCamera& cam, const glm::vec2& p,
			TouchPhase phase=TouchPhase::None);
        void update(double dt);
        void fixedUpdate(double dt);
        void render(gorn::RenderQueue& queue);

        std::shared_ptr<Entity> addChild(const std::shared_ptr<Entity>& child=nullptr);
        bool removeChild(const std::shared_ptr<Entity>& child);
        bool removeFromParent();

        const Children& getChildren() const;

        Component& addComponent(std::unique_ptr<Component> comp);

        template<typename C, typename... Args>
        C& addComponent(Args&&... args);

		Action& addAction(double duration, std::unique_ptr<Action> action);

		template<typename C, typename... Args>
		C& addAction(double duration, Args&&... args);

        template<typename C, typename... Args>
        std::shared_ptr<Entity> addChildWithComponent(Args&&... args);

        bool hasParent() const;
        Entity& getParent();
        const Entity& getParent() const;

		const Layers& getLayers() const;
		void setLayers(const Layers& layers);
		void setLayer(int layer);
		void clearLayers();
    };

    template<typename C, typename... Args>
    C& Entity::addComponent(Args&&... args)
    {
        auto ptr = new C(std::forward<Args>(args)...);
        addComponent(std::unique_ptr<Component>(ptr));
        return *ptr;
    }

	template<typename C, typename... Args>
	C& Entity::addAction(double duration, Args&&... args)
	{
		auto ptr = new C(std::forward<Args>(args)...);
		addAction(duration, std::unique_ptr<Action>(ptr));
		return *ptr;
	}

    template<typename C, typename... Args>
    std::shared_ptr<Entity> Entity::addChildWithComponent(Args&&... args)
    {
        auto child = addChild();
        child->addComponent<C>(std::forward<Args>(args)...);
        return child;
    }
}

#endif
