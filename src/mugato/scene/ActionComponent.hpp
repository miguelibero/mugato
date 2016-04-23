#ifndef __mugato__ActionComponent__
#define __mugato__ActionComponent__

#include <mugato/scene/Component.hpp>
#include <mugato/action/Action.hpp>
#include <memory>
#include <functional>

namespace mugato
{
    class ActionComponent : public Component
    {
	public:
		typedef std::function<void(void)> FinishCallback;
    private:
        std::weak_ptr<Entity> _entity;
		std::unique_ptr<Action> _action;
		double _duration;
		double _position;
		bool _started;
		FinishCallback _finished;
    public:
		ActionComponent(double duration, std::unique_ptr<Action> action,
			const FinishCallback& finished = nullptr);
        void onAddedToEntity(Entity& entity) override;
		void setFinishCallback(const FinishCallback& cb);
        void update(double dt) override;
		bool finished() const;
		double getDuration() const;
    };
}

#endif
