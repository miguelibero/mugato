#ifndef __mugato__CameraComponent__
#define __mugato__CameraComponent__

#include <mugato/scene/Component.hpp>
#include <functional>

namespace mugato
{
    class CameraComponent : public Component
    {
    private:
		glm::mat4 _view;
		glm::mat4 _proj;
		glm::mat4 _transform;

		void updateTransform();
    public:
		CameraComponent();
		CameraComponent(const glm::mat4& view, const glm::mat4& proj);

		void setViewTransform(const glm::mat4& view);
		void setProjectionTransform(const glm::mat4& proj);

		void beforeEntityChildrenRender(gorn::RenderQueue& queue) override;
		void afterEntityChildrenRender(gorn::RenderQueue& queue) override;

    };
}

#endif
