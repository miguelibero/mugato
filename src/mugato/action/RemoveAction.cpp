#include <mugato/action/RemoveAction.hpp>
#include <mugato/scene/Entity.hpp>

namespace mugato
{
	RemoveAction::RemoveAction()
	{
	}

	RemoveAction::RemoveAction(const std::weak_ptr<Entity>& ptr):
		_entity(ptr)
	{
	}

	void RemoveAction::finish(Entity& entity)
	{
		if(auto ptr = _entity.lock())
		{
			ptr->removeFromParent();
		}
		else
		{
			entity.removeFromParent();
		}
	}
}
