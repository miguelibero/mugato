
#ifndef __mugato__EmptyAction__
#define __mugato__EmptyAction__

#include <mugato/scene/Action.hpp>
#include <functional>

namespace mugato {
 
	class EmptyAction : public Action
	{
	public:
		typedef std::function<void()> CompleteCallback;
	private:
		CompleteCallback _complete;

	public:
		EmptyAction(
            const CompleteCallback& complete=nullptr);

		EmptyAction& withComplete(const CompleteCallback& complete);

		void finish(Entity& entity) override;
	};
}

#endif
