
#ifndef __mugato__SequenceAction__
#define __mugato__SequenceAction__

#include <mugato/action/Action.hpp>
#include <vector>
#include <memory>

namespace mugato
{

	class SequenceAction : public Action
	{
	private:
		struct Element
		{
			double start;
			double duration;
			bool running;
			std::unique_ptr<Action> action;
		};
		std::vector<Element> _actions;
		double _duration;
		double _lastPosition;

		std::vector<Element>::iterator find(double pos);
		void updateThrough(Element& elm, Entity& entity);
	public:
		SequenceAction();

		void add(double duration, std::unique_ptr<Action> action);
		void clear();

		template<typename A, typename... Args>
		A& add(double duration, Args&&... args);

		void start(Entity& entity) override;
		void update(Entity& entity, double pos) override;
		void finish(Entity& entity) override;
	};

	template<typename A, typename... Args>
	A& SequenceAction::add(double duration, Args&&... args)
	{
		auto ptr = new A(std::forward<Args>(args)...);
		add(duration, std::unique_ptr<Action>(ptr));
		return *ptr;
	}
}

#endif
