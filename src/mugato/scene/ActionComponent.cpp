#include <mugato/scene/ActionComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <algorithm>

namespace mugato
{
    ActionState::ActionState(float duration,
        std::unique_ptr<Action> action):
    _action(std::move(action)),
    _duration(duration),
    _position(0.0f),
    _started(false)
    {
    }
    void ActionState::update(double dt)
    {
        _position += dt;
    }

    void ActionState::update(Entity& entity)
    {
        if(!_started)
        {
            _action->start(entity);
            _started = true;
        }
        if(finished())
        {
            _action->finish(entity);
            return;
        }
        else
        {
            _action->update(entity, _position / _duration);
        }
    }

    bool ActionState::finished() const
    {
        return _position >= _duration;
    }

    void ActionComponent::clear()
    {
        _actions.clear();
    }

    void ActionComponent::add(float duration,
        std::unique_ptr<Action> action)
    {
        _actions.push_back(ActionState(duration, std::move(action)));
    }

    void ActionComponent::onAddedToEntity(Entity& entity)
    {
        _entity = entity.getSharedPtr();
    }

    void ActionComponent::update(double dt)
    {
        for(auto itr = _actions.begin(); itr != _actions.end(); ++itr)
        {
            itr->update(dt);
        }
        if(auto ptr = _entity.lock())
        {
            for(auto itr = _actions.begin(); itr != _actions.end(); ++itr)
            {
                itr->update(*ptr);
            }
        }
        _actions.erase(std::remove_if(_actions.begin(), _actions.end(),
            [&dt](ActionState& state){
                return state.finished();
            }), _actions.end());
    }

    void ActionComponent::render(gorn::RenderQueue& queue)
    {
    }
}

