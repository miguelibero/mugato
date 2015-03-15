#ifndef __mugato__Entity__
#define __mugato__Entity__

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <mugato/base/OcTree.hpp>

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
        typedef glm::vec3 Vector;
        typedef glm::vec2 Vector2;
        typedef glm::mat4 Transform;
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
        Vector _position;
        Vector _rotation;
        Vector _scale;
        Vector _pivot;
        Vector _size;
        Rectangle _area;

        void updateTransform();
    public:

        Entity();

        std::shared_ptr<Entity> getSharedPtr();

        void setContext(Context& ctx);
        Context& getContext() const;

        const Vector& getPosition() const;
        const Vector& getRotation() const;
        const Vector& getScale() const;
        const Vector& getPivot() const;
        const Vector& getSize() const;
        const Rectangle& getArea() const;

        void setPosition(const Vector& val);
        void setRotation(const Vector& val);
        void setScale(const Vector& val);
        void setPivot(const Vector& val);
        void setSize(const Vector& val);

        void setPosition(const Vector2& val);
        void setRotation(const Vector2& val);
        void setScale(const Vector2& val);
        void setPivot(const Vector2& val);
        void setSize(const Vector2& val);

        void setRotation(float val);
        void setScale(float val);

        void update(double dt);
        void render(gorn::RenderQueue& queue);

        std::shared_ptr<Entity> addChild(std::shared_ptr<Entity> child=nullptr);
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
