#include <mugato/scene/Entity.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/base/Exception.hpp>

namespace mugato
{
    Entity::Entity():
    _transformDirty(false),
    _ctx(nullptr)
    {
    }

    void Entity::setContext(Context& ctx)
    {
        _ctx = &ctx;
    }

    Context& Entity::getContext() const
    {
        if(_ctx)
        {
            return *_ctx;
        }
        else if(hasParent())
        {
            return getParent().getContext();
        }
        else
        {
            throw Exception("Could not find context");
        }
    }

    const glm::vec2& Entity::getPosition() const
    {
        return _position;
    }

    const glm::vec2& Entity::getRotation() const
    {
        return _rotation;
    }

    const glm::vec2& Entity::getScale() const
    {
        return _scale;
    }

    void Entity::setPosition(const glm::vec2& val)
    {
        if(_position != val)
        {
            _transformDirty = true;
            _position = val;
        }
    }

    void Entity::setRotation(const glm::vec2& val)
    {
        if(_rotation != val)
        {
            _transformDirty = true;
            _rotation = val;
        }
    }

    void Entity::setScale(const glm::vec2& val)
    {
        if(_scale != val)
        {
            _transformDirty = true;
            _scale = val;
        }
    }

    void Entity::update(double dt)
    {
        for(auto& comp : _components)
        {
            comp->update(dt);
        }
        for(auto& child : _children)
        {
            child->update(dt);
        }
    }

    void Entity::render(gorn::RenderQueue& queue)
    {
        for(auto& comp : _components)
        {
            comp->render(queue);
        }
        for(auto& child : _children)
        {
            child->render(queue);
        }
    }

    Entity& Entity::addChild(std::shared_ptr<Entity> child)
    {
        if(child == nullptr)
        {
            child = std::make_shared<Entity>();
        }
        child->_parent = shared_from_this();
        _children.push_back(child);
        return *child;
    }

    Component& Entity::addComponent(std::unique_ptr<Component> comp)
    {
        auto ptr = comp.get();
        comp->onAddedToEntity(*this);
        _components.push_back(std::move(comp));
        return *ptr;
    }

    bool Entity::hasParent() const
    {
        return !_parent.expired();
    }

    Entity& Entity::getParent()
    {
        return *_parent.lock();
    }

    const Entity& Entity::getParent() const
    {
        return *_parent.lock();
    }
}

