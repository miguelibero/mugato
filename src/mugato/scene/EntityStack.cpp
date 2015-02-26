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

    void EntityStack::update(double dt)
    {
        if(!_stack.empty())
        {
            _stack.back()->update(dt);
        }
    }

    void EntityStack::render(gorn::RenderQueue& queue)
    {
        if(!_stack.empty())
        {
            _stack.back()->render(queue);
        }
    }

    Entity& EntityStack::push(std::shared_ptr<Entity> entity)
    {
        if(entity == nullptr)
        {
            entity = std::make_shared<Entity>();
        }
        if(_ctx != nullptr)
        {
            entity->setContext(*_ctx);
        }
        _stack.push_back(entity);
        return get();
    }

    Entity& EntityStack::get()
    {
        if(_stack.empty())
        {
            throw Exception("empty stack");
        }
        return *_stack.back();
    }

    void EntityStack::pop()
    {
        if(_stack.empty())
        {
            throw Exception("empty stack");
        }
        _stack.pop_back();
    }

    Entity& EntityStack::set(std::shared_ptr<Entity> entity)
    {
        _stack.clear();
        return push(entity);
    }
}

