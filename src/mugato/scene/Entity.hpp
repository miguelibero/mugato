#ifndef __mugato__Entity__
#define __mugato__Entity__

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <mugato/base/OcTree.hpp>
#include <mugato/scene/Component.hpp>
#include <mugato/scene/EntityTransform.hpp>

namespace gorn
{
    class RenderQueue;
}

namespace mugato
{
    class Context;
    class Component;

    class Entity : public std::enable_shared_from_this<Entity>
    {
    public:
        typedef EntityTransform Transform;
        typedef OcTree<std::shared_ptr<Entity>> Children;
        typedef std::weak_ptr<Entity> Parent;
        typedef std::vector<std::unique_ptr<Component>> Components;
    private:
        Components _components;
        Children _children;
        Parent _parent;
        Transform _transform;
        bool _transformDirty;
        Context* _ctx;
        
        void updateTransform();
    public:

        Entity();

        std::shared_ptr<Entity> getSharedPtr();

        void setContext(Context& ctx);
        Context& getContext() const;

        Transform& getTransform();
        const Transform& getTransform() const;

        void update(double dt);
        void fixedUpdate(double dt);
        void render(gorn::RenderQueue& queue);

        std::shared_ptr<Entity> addChild(const std::shared_ptr<Entity>& child=nullptr);
        bool removeFromParent();

        const Children& getChildren() const;
        Children& getChildren();

        Component& addComponent(std::unique_ptr<Component> comp);

        template<typename C, typename... Args>
        C& addComponent(Args&&... args);

        bool hasParent() const;
        Entity& getParent();
        const Entity& getParent() const;
    };

    template<typename C, typename... Args>
    C& Entity::addComponent(Args&&... args)
    {
        auto ptr = new C(std::forward<Args>(args)...);
        addComponent(std::unique_ptr<Component>(ptr));
        return *ptr;
    }
}

#endif
