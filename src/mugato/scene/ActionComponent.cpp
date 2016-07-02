#include <mugato/scene/ActionComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <algorithm>

namespace mugato
{
	ActionComponent::ActionComponent(double duration,
		std::unique_ptr<Action> action,
		const FinishCallback& finished):
	_action(std::move(action)),
	_duration(duration),
	_position(0.0),
	_started(false),
	_finished(finished)
	{
	}

    void ActionComponent::onAddedToEntity(Entity& entity)
    {
		_entity = entity.getSharedPtr();
    }

	void ActionComponent::setFinishCallback(const FinishCallback& cb)
	{
		_finished = cb;
	}

	double ActionComponent::getDuration() const
	{
		return _duration;
	}

	bool ActionComponent::finished() const
	{
		return _position >= _duration;
	}

    void ActionComponent::update(double dt)
    {
		_position += dt;
		auto entity = _entity.lock();
		if(entity == nullptr)
		{
			return;
		}
		if(finished())
		{
			if(_started)
			{
				_action->finish(*entity);
				if (_finished != nullptr)
				{
					_finished();
				}
				_started = false;
			}
			if(auto ptr = _entity.lock())
			{
				ptr->removeComponent(*this);
			}
			return;
		}
		else
		{
			if (!_started)
			{
				_action->start(*entity);
				_started = true;
			}
			_action->update(*entity, _position / _duration);
		}
    }

}

