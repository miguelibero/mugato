#include <mugato/scene/Entity.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/base/Exception.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <gorn/render/RenderCommand.hpp>

namespace mugato
{
    Entity::Entity()
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

    const Entity::Transform& Entity::getTransform() const
    {
        return _transform;
    }

    Entity::Transform& Entity::getTransform()
    {
        return _transform;
    }

    std::shared_ptr<Entity> Entity::getSharedPtr()
    {
        return shared_from_this();
    }

    void Entity::updateTransform()
    {
        if(_transform.update())
        {
            _children.setArea(Rectangle(
                glm::vec3(0.0f), _transform.getSize()));
            if(auto parent = _parent.lock())
            {
                parent->addChild(getSharedPtr());
            }
        }
    }

    void Entity::update(double dt)
    {
        updateTransform();
        for(auto& comp : _components)
        {
            comp->update(dt);
        }
        _children.adjust();
        Children::Elements elements;
        _children.find(elements);
        for(auto& elm : elements)
        {
            elm.getContent()->update(dt);
        }
    }

    void Entity::render(gorn::RenderQueue& queue)
    {
        queue.addCommand()
            .withTransformMode(gorn::RenderCommand::TransformMode::PushCheckpoint);
        queue.addCommand()
            .withTransform(_transform.getMatrix())
            .withTransformMode(gorn::RenderCommand::TransformMode::PushLocal);     
        for(auto& comp : _components)
        {
            comp->render(queue);
        }
        Children::Elements elements;
        _children.find(elements);
        for(auto& elm : elements)
        {
            elm.getContent()->render(queue);
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
        updateTransform();
        child->updateTransform();
        child->_parent = getSharedPtr();
        _children.insert(Children::Element(child->getTransform().getArea(), child));
        return child;
    }

    bool Entity::removeFromParent()
    {
        if(auto parent = _parent.lock())
        {
            auto ptr = getSharedPtr();
            return parent->_children.clear([&ptr](const Children::Element& elm){
                return elm.getContent() == ptr;
            });
        }
        return false;
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

    const Entity::Children& Entity::getChildren() const
    {
        return _children;
    }

    Entity::Children& Entity::getChildren()
    {
        return _children;
    }
}

