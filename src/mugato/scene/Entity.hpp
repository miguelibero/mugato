#ifndef __mugato__Entity__
#define __mugato__Entity__

#include <glm/glm.hpp>
#include <memory>
#include <vector>

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
        typedef std::vector<std::shared_ptr<Entity>> Children;
        typedef std::weak_ptr<Entity> Parent;
        typedef std::vector<std::unique_ptr<Component>> Components;
    private:
        Components _components;
        Children _children;
        Parent _parent;
        glm::vec2 _position;
        glm::vec2 _rotation;
        glm::vec2 _scale;
        glm::mat4 _transform;
        bool _transformDirty;
        Context* _ctx;
    public:

        Entity();

        void setContext(Context& ctx);
        Context& getContext() const;

        const glm::vec2& getPosition() const;
        const glm::vec2& getRotation() const;
        const glm::vec2& getScale() const;

        void setPosition(const glm::vec2& val);
        void setRotation(const glm::vec2& val);
        void setScale(const glm::vec2& val);

        void update(double dt);
        void render(gorn::RenderQueue& queue);

        Entity& addChild(std::shared_ptr<Entity> child=nullptr);
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
