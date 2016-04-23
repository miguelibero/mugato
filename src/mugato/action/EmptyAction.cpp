#include <mugato/action/EmptyAction.hpp>
#include <mugato/scene/Entity.hpp>

namespace mugato {

	EmptyAction::EmptyAction(const CompleteCallback& complete) :
	_complete(complete)
	{
	}

	EmptyAction& EmptyAction::withComplete(const CompleteCallback& complete)
	{
		_complete = complete;
        return *this;
	}

	void EmptyAction::finish(Entity& entity)
	{
        if(_complete != nullptr)
        {
            _complete();
        }
    }
}
