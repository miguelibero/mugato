#include <mugato/action/SequenceAction.hpp>
#include <mugato/scene/Entity.hpp>

namespace mugato {

	SequenceAction::SequenceAction():
	_duration(0.0),
	_lastPosition(0.0)
	{
	}

	void SequenceAction::add(double duration, std::unique_ptr<Action> action)
	{
		auto start = _duration;
		_duration += duration;
		_actions.push_back(Element{
			start, duration, false, std::move(action) });
	}

	void SequenceAction::clear()
	{
		_duration = 0.0;
		_actions.clear();
	}

	void SequenceAction::start(Entity& entity)
	{
		_lastPosition = 0.0;

	}

	std::vector<SequenceAction::Element>::iterator SequenceAction::find(double pos)
	{
		auto itr = _actions.begin();
		for(; itr != _actions.end(); ++itr)
		{
			double end = itr->start + itr->duration;
			if(itr->start <= pos && end > pos)
			{
				return itr;
			}
		}
		return itr;
	}

	void SequenceAction::updateThrough(Element& elm, Entity& entity)
	{
		if(!elm.running)
		{
			elm.action->start(entity);
		}
		elm.action->finish(entity);
		elm.running = false;
	}

	void SequenceAction::update(Entity& entity, double p)
	{
		double pos = p * _duration;
		double lpos = _lastPosition * _duration;
		_lastPosition = p;
		auto srcItr = find(lpos);
		auto dstItr = find(pos);

		if(dstItr > srcItr)
		{
			for(auto itr = srcItr; itr != dstItr; itr++)
			{
				updateThrough(*itr, entity);
			}
		}
		else if(dstItr < srcItr)
		{
			for(auto itr = srcItr; itr != dstItr; itr--)
			{
				updateThrough(*itr, entity);
			}
		}
		if(dstItr != _actions.end())
		{
			if(!dstItr->running)
			{
				dstItr->running = true;
				dstItr->action->start(entity);
			}
			dstItr->action->update(entity, pos - dstItr->start);
		}
	}

	void SequenceAction::finish(Entity& entity)
	{
		update(entity, 1.0);
    }
}
