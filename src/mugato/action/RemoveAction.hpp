
#ifndef __mugato__RemoveAction__
#define __mugato__RemoveAction__

#include <mugato/action/Action.hpp>
#include <mugato/scene/Entity.hpp>
#include <memory>

namespace mugato {
 
	class RemoveAction : public Action
	{
	private:
		std::weak_ptr<Entity> _entity;

	public:
		RemoveAction();
		RemoveAction(const std::weak_ptr<Entity>& ptr);
		void finish(Entity& entity) override;
	};
}

#endif
