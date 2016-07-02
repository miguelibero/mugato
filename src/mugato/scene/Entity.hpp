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
	class Ray;
}

namespace mugato
{
    class Context;
    class Component;
	class Action;

	struct EntityComponentData
	{
		Component::type_t id;
		std::unique_ptr<Component> component;
	};

    class Entity final : public std::enable_shared_from_this<Entity>
    {
    public:
        typedef EntityTouchPhase TouchPhase;
        typedef EntityTransform Transform;
		typedef EntityComponentData ComponentData;
        typedef std::vector<std::shared_ptr<Entity>> Children;
        typedef std::vector<std::weak_ptr<Entity>> TouchedChildren;
        typedef std::weak_ptr<Entity> Parent;
        typedef std::vector<ComponentData> Components;
		typedef std::vector<int> Layers;
    private:
        Context* _ctx;
        Components _componentsToAdd;
		std::vector<const Component*> _removedComponents;
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
		void removePendingComponents();
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

        Component& addComponent(Component::type_t type, std::unique_ptr<Component> comp);

        template<typename C, typename... Args>
        C& addComponent(Args&&... args);

		template<typename C, typename... Args>
		C& getOrAddComponent(Args&&... args);

		template<typename C>
		bool hasComponent() const;

		template<typename C>
		C& getComponent();

		template<typename C>
		const C& getComponent() const;

		template<typename C>
		std::vector<C*> getComponents();

		template<typename C>
		std::vector<const C*> getComponents() const;

		template<typename C>
		bool removeComponents();

		bool hasComponent(Component::type_t type) const;
		Component& getComponent(Component::type_t type);
		const Component& getComponent(Component::type_t type) const;
		std::vector<Component*> getComponents(Component::type_t type);
		std::vector<const Component*> getComponents(Component::type_t type) const;
		bool removeComponents(Component::type_t type);
		bool removeComponent(const Component& comp);

		Action& addAction(double duration, std::unique_ptr<Action> action);

		template<typename C, typename... Args>
		C& addAction(double duration, Args&&... args);

		void clearActions();

        template<typename C, typename... Args>
        std::shared_ptr<Entity> addChildWithComponent(Args&&... args);

        bool hasParent() const;
        Entity& getParent();
        const Entity& getParent() const;

		const Layers& getLayers() const;
		void setLayers(const Layers& layers);
		void setLayer(int layer);
		void clearLayers();

		bool hitBy(const gorn::Ray& ray) const;
		bool hitBy(const gorn::Ray& ray, glm::vec3& hp) const;
    };

    template<typename C, typename... Args>
    C& Entity::addComponent(Args&&... args)
    {
        auto ptr = new C(std::forward<Args>(args)...);
        addComponent(ComponentTypeId<C>(), std::unique_ptr<Component>(ptr));
        return *ptr;
    }

	template<typename C, typename... Args>
	C& Entity::getOrAddComponent(Args&&... args)
	{
		if(hasComponent<C>())
		{
			return getComponent<C>();
		}
		return addComponent<C, Args...>(std::forward<Args>(args)...);
	}

	template<typename C, typename... Args>
	C& Entity::addAction(double duration, Args&&... args)
	{
		auto ptr = new C(std::forward<Args>(args)...);
		addAction(duration, std::unique_ptr<Action>(ptr));
		return *ptr;
	}

	template<typename C>
	bool Entity::hasComponent() const
	{
		return hasComponent(ComponentTypeId<C>());
	}

	template<typename C>
	C& Entity::getComponent()
	{
		return static_cast<C&>(getComponent(ComponentTypeId<C>()));
	}

	template<typename C>
	const C& Entity::getComponent() const
	{
		return static_cast<const C&>(getComponent(ComponentTypeId<C>()));
	}

	template<typename C>
	std::vector<C*> Entity::getComponents()
	{
		auto v = getComponents(ComponentTypeId<C>());
		return std::vector<C*>(v.begin(), v.end());
	}

	template<typename C>
	std::vector<const C*> Entity::getComponents() const
	{
		auto v = getComponents(ComponentTypeId<C>());
		return std::vector<const C*>(v.begin(), v.end());
	}

	template<typename C>
	bool Entity::removeComponents()
	{
		return removeComponents(ComponentTypeId<C>());
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
