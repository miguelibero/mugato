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
        glm::vec3 _dir;
        glm::vec3 _pos;
        glm::vec3 _up;
        bool _updateView;

		void cleanContext();
	public:
		CameraComponent();
		~CameraComponent();

		CameraComponent& withProjection(const glm::mat4& proj);
        CameraComponent& withDirection(const glm::vec3& dir);
		CameraComponent& withBlendMode(const gorn::BlendMode& blend);
		CameraComponent& withUniformValue(const std::string& name, const gorn::UniformValue& value);
		CameraComponent& withUniformValues(const gorn::UniformValueMap& values);

		void onAssignedToContext(Context& ctx) override;
		void onAddedToEntity(Entity& entity) override;
		void onEntityTransformChanged(Entity& entity) override;
        void update(double dt) override;
    };
}

#endif
