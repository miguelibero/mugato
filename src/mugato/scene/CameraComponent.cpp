#include <mugato/scene/CameraComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/base/Context.hpp>

namespace mugato
{
    CameraComponent::CameraComponent():
	_camera(std::make_shared<gorn::RenderCamera>()),
	_ctx(nullptr)
    {
    }

	CameraComponent::~CameraComponent()
	{
		cleanContext();
	}

	void CameraComponent::cleanContext()
	{
		if (_ctx != nullptr && _camera != nullptr)
		{
			_ctx->getGorn().getQueue().removeCamera(_camera);
		}
	}

	CameraComponent& CameraComponent::withProjection(const glm::mat4& proj)
	{
		_camera->withProjection(proj);
		return *this;
	}

	CameraComponent& CameraComponent::withBlendMode(const gorn::BlendMode& blend)
	{
		_camera->withBlendMode(blend);
		return *this;
	}

	CameraComponent& CameraComponent::withUniformValue(const std::string& name, const gorn::UniformValue& value)
	{
		_camera->withUniformValue(name, value);
		return *this;
	}

	CameraComponent& CameraComponent::withUniformValues(const gorn::UniformValueMap& values)
	{
		_camera->withUniformValues(values);
		return *this;
	}

	void CameraComponent::onAssignedToContext(Context& ctx)
	{
		cleanContext();
		ctx.getGorn().getQueue().addCamera(_camera);
		_ctx = &ctx;
	}

	void CameraComponent::onAddedToEntity(Entity& entity)
	{
		onEntityTransformChanged(entity);
	}

	void CameraComponent::onEntityTransformChanged(Entity& entity)
	{
		auto mat = entity.getModelMatrix();
		_camera->withView(mat);
	}
}
