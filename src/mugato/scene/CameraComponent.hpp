#ifndef __mugato__CameraComponent__
#define __mugato__CameraComponent__

#include <mugato/scene/Component.hpp>
#include <gorn/render/RenderCamera.hpp>
#include <memory>

namespace mugato
{
    class CameraComponent : public Component
    {
    private:
		std::shared_ptr<gorn::RenderCamera> _camera;
		Context* _ctx;
		glm::vec3 _lookAt;
		glm::vec3 _pos;
		glm::mat4 _model;
        bool _updateView;

		void cleanContext();
	public:
		CameraComponent();
		~CameraComponent();

		CameraComponent& withProjection(const glm::mat4& proj);
		CameraComponent& withLookAt(const glm::vec3& lookAt);
		CameraComponent& withBlendMode(const gorn::BlendMode& blend);
		CameraComponent& withUniformValue(const std::string& name, const gorn::UniformValue& value);
		CameraComponent& withUniformValues(const gorn::UniformValueMap& values);

		void onAssignedToContext(Context& ctx) override;
		void onAddedToEntity(Entity& entity) override;
		void onEntityParentTransformChanged(Entity& entity, Entity& parent) override;
		void onEntityTransformChanged(Entity& entity) override;
		void onEntityLayersChanged(Entity& entity) override;
        void update(double dt) override;
    };
}

#endif
