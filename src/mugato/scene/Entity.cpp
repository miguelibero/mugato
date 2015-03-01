#include <mugato/scene/Entity.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/base/Exception.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <gorn/render/RenderCommand.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp> 

namespace mugato
{
    Entity::Entity():
    _transformDirty(true),
    _ctx(nullptr),
    _position(0.0f),
    _rotation(0.0f),
    _scale(1.0f, 1.0f, 0.0f),
    _pivot(0.0f)
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

    const Entity::Vector& Entity::getPosition() const
    {
        return _position;
    }

    const Entity::Vector& Entity::getRotation() const
    {
        return _rotation;
    }

    const Entity::Vector& Entity::getScale() const
    {
        return _scale;
    }

    const Entity::Vector& Entity::getPivot() const
    {
        return _pivot;
    }

    void Entity::setPosition(const Vector& val)
    {
        if(_position != val)
        {
            _transformDirty = true;
            _position = val;
        }
    }

    void Entity::setPosition(const Vector2& val)
    {
        setPosition(Vector(val.x, val.y, 0.0f));
    }

    void Entity::setRotation(const Vector2& val)
    {
        setRotation(Vector(val.x, val.y, 0.0f));
    }

    void Entity::setScale(const Vector2& val)
    {
        setScale(Vector(val.x, val.y, 0.0f));
    }

    void Entity::setPivot(const Vector2& val)
    {
        setPivot(Vector(val.x, val.y, 0.0f));
    }

    void Entity::setRotation(float val)
    {
        setRotation(glm::vec3(0.0f, val, 0.0f));
    }

    void Entity::setScale(float val)
    {
        setScale(glm::vec2(val));
    }

    void Entity::setRotation(const Vector& val)
    {
        if(_rotation != val)
        {
            _transformDirty = true;
            _rotation = val;
        }
    }

    void Entity::setScale(const Vector& val)
    {
        if(_scale != val)
        {
            _transformDirty = true;
            _scale = val;
        }
    }


    void Entity::setPivot(const Vector& val)
    {
        if(_pivot != val)
        {
            _transformDirty = true;
            _pivot = val;
        }
    }

    std::shared_ptr<Entity> Entity::getSharedPtr()
    {
        return shared_from_this();
    }

    void Entity::update(double dt)
    {
        if(_transformDirty)
        {

            _transform = glm::translate(glm::mat4(), _position)                
                * glm::translate(glm::mat4(), _pivot)
                * glm::scale(glm::mat4(), _scale)
                * glm::orientate4(_rotation)
                * glm::translate(glm::mat4(), -_pivot)
                ;

            _transformDirty = false;
        }
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
        queue.addCommand()
            .withTransformMode(gorn::RenderCommand::TransformMode::PushCheckpoint);
        queue.addCommand()
            .withTransform(_transform)
            .withTransformMode(gorn::RenderCommand::TransformMode::PushLocal);     
        for(auto& comp : _components)
        {
            comp->render(queue);
        }
        for(auto& child : _children)
        {
            child->render(queue);
        }
        queue.addCommand()
            .withTransformMode(gorn::RenderCommand::TransformMode::PopCheckpoint);
    }

    std::shared_ptr<Entity> Entity::addChild(std::shared_ptr<Entity> child)
    {
        if(child == nullptr)
        {
            child = std::make_shared<Entity>();
        }
        child->_parent = getSharedPtr();
        _children.push_back(child);
        return child;
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

