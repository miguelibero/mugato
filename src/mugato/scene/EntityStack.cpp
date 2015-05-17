#include <mugato/scene/EntityStack.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/base/Exception.hpp>

namespace mugato
{
    EntityStack::EntityStack():
    _ctx(nullptr)
    {
    }

    void EntityStack::setContext(Context& ctx)
    {
        _ctx = &ctx;
    }

    const EntityTransform& EntityStack::getTransform() const
    {
        return _transform;
    }

    EntityTransform& EntityStack::getTransform()
    {
        return _transform;
    }

    void EntityStack::onAddedToEntity(Entity& entity)
    {
        setContext(entity.getContext());
        _entity = entity.getSharedPtr();
        onEntityTransformChanged(entity);
    }

    void EntityStack::onEntityTransformChanged(Entity& entity)
    {
    }

    void EntityStack::onEntityTouched(Entity& entity, const glm::vec2& p)
    {
        touch(p);
    }

    void EntityStack::touch(const glm::vec2& p)
    {
        if(!_stack.empty())
        {
            auto ep = p;
            if(auto ptr = _entity.lock())
            {
                ep = ptr->getTransform().getParentToLocalPoint(ep);
            }
            _stack.back()->touch(ep);
        }
    }

    void EntityStack::update(double dt)
    {
        _transform.update();
        if(!_stack.empty())
        {
            _stack.back()->update(dt);
        }
    }


    void EntityStack::fixedUpdate(double dt)
    {
        if(!_stack.empty())
        {
            _stack.back()->fixedUpdate(dt);
        }
    }

    void EntityStack::render(gorn::RenderQueue& queue)
    {
        if(!_stack.empty())
        {
            _stack.back()->render(queue);
        }
    }

    std::shared_ptr<Entity> EntityStack::push(std::shared_ptr<Entity> entity)
    {
        if(entity == nullptr)
        {
            entity = std::make_shared<Entity>();
            entity->getTransform() = _transform;
        }
        if(_ctx != nullptr)
        {
            entity->setContext(*_ctx);
        }
        _stack.push_back(entity);
        return get();
    }

    std::shared_ptr<Entity> EntityStack::get()
    {
        if(_stack.empty())
        {
            throw Exception("empty stack");
        }
        return _stack.back();
    }

    void EntityStack::pop()
    {
        if(_stack.empty())
        {
            throw Exception("empty stack");
        }
        _stack.pop_back();
    }

    std::shared_ptr<Entity> EntityStack::set(std::shared_ptr<Entity> entity)
    {
        _stack.clear();
        return push(entity);
    }
}

