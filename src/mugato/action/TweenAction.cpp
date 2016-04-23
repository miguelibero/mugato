#include <mugato/action/TweenAction.hpp>
#include <mugato/scene/Entity.hpp>

namespace mugato {

	TweenAction::TweenAction(const EntityTransform& end,
            const CompleteCallback& complete,
            const EasingCallback& easing) :
	_complete(complete), _easing(easing), _end(end)
	{
		if(_easing == nullptr)
		{
			_easing = std::bind(&VectorEasing::linear,
                std::placeholders::_1,
                std::placeholders::_2,
                std::placeholders::_3);
		}
	}

    TweenAction& TweenAction::withEnd(const EntityTransform& end)
    {
        _end = end;
        return *this;
    }
    
	TweenAction& TweenAction::withEasing(const EasingCallback& easing)
	{
		_easing = easing;
        return *this;
	}

	TweenAction& TweenAction::withComplete(const CompleteCallback& complete)
	{
		_complete = complete;
        return *this;
	}

	void TweenAction::start(Entity& entity)
	{
	    _start = entity.getTransform();
    }

	void TweenAction::update(Entity& entity, double p)
	{
        auto& t = entity.getTransform();
        Vector sv, ev;

        sv = _start.getPosition();
        ev = _end.getPosition();
        ev = _easing(p, sv, ev - sv);
        t.setPosition(ev);

        sv = _start.getRotation();
        ev = _end.getRotation();
        ev = _easing(p, sv, ev - sv);
        t.setRotation(ev);

        sv = _start.getScale();
        ev = _end.getScale();
        ev = _easing(p, sv, ev - sv);
        t.setScale(ev);

        sv = _start.getPivot();
        ev = _end.getPivot();
        ev = _easing(p, sv, ev - sv);
        t.setPivot(ev);

        sv = _start.getSize();
        ev = _end.getSize();
        ev = _easing(p, sv, ev - sv);
        t.setSize(ev);
	}

	void TweenAction::finish(Entity& entity)
	{
        update(entity, 1.0f);
        if(_complete != nullptr)
        {
            _complete();
        }
    }


}
