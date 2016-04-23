
#ifndef __mugato__TweenAction__
#define __mugato__TweenAction__

#include <mugato/base/Easing.hpp>
#include <mugato/action/Action.hpp>
#include <mugato/scene/EntityTransform.hpp>
#include <functional>

namespace mugato {
 
	class TweenAction : public Action
	{
	public:
		typedef std::function<void()> CompleteCallback;
        typedef EntityTransform::Vector Vector;
        typedef Easing<Vector> VectorEasing;
		typedef VectorEasing::Callback EasingCallback;
	private:
		CompleteCallback _complete;
		EasingCallback _easing;
		EntityTransform _start;
		EntityTransform _end;

	public:
		TweenAction(const EntityTransform& end,
            const CompleteCallback& complete=nullptr,
            const EasingCallback& easing=nullptr);

        TweenAction& withEnd(const EntityTransform& end);
		TweenAction& withEasing(const EasingCallback& easing);
		TweenAction& withComplete(const CompleteCallback& complete);

		void start(Entity& entity) override;
		void update(Entity& entity, double pos) override;
		void finish(Entity& entity) override;
	};
}

#endif
