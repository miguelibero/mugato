#include <mugato/scene/CameraComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/base/Context.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

namespace mugato
{
    CameraComponent::CameraComponent():
	_camera(std::make_shared<gorn::RenderCamera>()),
	_ctx(nullptr),
    _updateView(false)
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

    CameraComponent& CameraComponent::withLookAt(const glm::vec3& lookAt)
    {
        if(_lookAt != lookAt)
        {
			_lookAt = lookAt;
            _updateView = true;
        }
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

	void CameraComponent::onEntityParentTransformChanged(Entity& entity, Entity& parent)
	{
		onEntityTransformChanged(entity);
	}

	void CameraComponent::onEntityTransformChanged(Entity& entity)
	{
		auto model = entity.getModelMatrix();
		if(_model != model)
        {
			_model = model;
            _updateView = true;
        }
		auto& pos = entity.getTransform().getPosition();
		if(_pos != pos)
		{
			_pos = pos;
			_updateView = true;
		}
		onEntityLayersChanged(entity);
	}

	void CameraComponent::onEntityLayersChanged(Entity& entity)
	{
		_camera->withLayers(entity.getLayers());
	}

    void CameraComponent::update(double dt)
    {
        if(_updateView)
        {
			auto p = glm::vec3(_model * glm::vec4(0, 0, 0, 1));
			auto c = glm::vec3(_model * glm::vec4(_lookAt - _pos, 1.0f));
			auto n = glm::transpose(glm::inverse(glm::mat3(_model)));
			auto u = n * glm::vec3(0, 1, 0);
            _camera->withView(glm::lookAt(
				p, c, u));
            _updateView = false;
        }
    }
}
