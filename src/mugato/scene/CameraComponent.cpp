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

    CameraComponent& CameraComponent::withDirection(const glm::vec3& dir)
    {
        if(_dir != dir)
        {
            _dir = dir;
            _updateView = true;
        }
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
		auto& pos = entity.getTransform().getPosition();
        auto mat = glm::orientate4(entity.getTransform().getRotation());
        auto up = glm::vec3(mat * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        if(_pos != pos || _up != up)
        {
            _pos = pos;
            _up = up;
            _updateView = true;
        }
	}

    void CameraComponent::update(double dt)
    {
        if(_updateView)
        {
            _camera->withView(glm::lookAt(_pos, _pos + _dir, _up));
            _updateView = false;
        }
    }
}
