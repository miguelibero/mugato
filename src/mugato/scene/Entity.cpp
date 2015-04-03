#include <mugato/scene/Entity.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/base/Exception.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <gorn/render/RenderCommand.hpp>

namespace mugato
{
    Entity::Entity():
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
            _children.setArea(gorn::Rect(
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
        Children::Elements elements;
        _children.find(elements);
        for(auto& elm : elements)
        {
            elm.getContent()->update(dt);
        }
    }

    void Entity::fixedUpdate(double dt)
    {
        for(auto& comp : _components)
        {
            comp->fixedUpdate(dt);
        }
        Children::Elements elements;
        _children.find(elements);
        for(auto& elm : elements)
        {
            elm.getContent()->fixedUpdate(dt);
        }

        _children.adjust(false);
        _components.erase(std::remove_if(_components.begin(),
            _components.end(), [](const std::unique_ptr<Component>& c){
                return c->hasFinished();
            }), _components.end());
    }

    void Entity::render(gorn::RenderQueue& queue)
    {
        queue.addCommand()
            .withBounding(_transform.getArea(),
                gorn::RenderCommand::BoundingMode::Start)
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
            .withBoundingMode(
                gorn::RenderCommand::BoundingMode::End)
            .withTransformMode(gorn::RenderCommand::TransformMode::PopCheckpoint);
    }

    std::shared_ptr<Entity> Entity::addChild(const std::shared_ptr<Entity>& child)
    {
        if(child == nullptr)
        {
            return addChild(std::make_shared<Entity>());
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

