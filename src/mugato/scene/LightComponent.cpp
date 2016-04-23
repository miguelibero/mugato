
#include <mugato/scene/LightComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/base/Context.hpp>

namespace mugato
{
	LightComponent::LightComponent():
	_light(std::make_shared<Light>()),
	_ctx(nullptr)
	{
	}

	LightComponent::~LightComponent()
	{
		cleanContext();
	}

	LightComponent& LightComponent::withType(Type type)
	{
		_light->withType(type);
		return *this;
	}

	LightComponent& LightComponent::withColor(const glm::vec3& color)
	{
		_light->withColor(color);
		return *this;
	}

	LightComponent& LightComponent::withDirection(const glm::vec3& dir)
	{
		_direction = dir;
		return *this;
	}

	LightComponent& LightComponent::withAttenuation(float attenuation)
	{
		_light->withAttenuation(attenuation);
		return *this;
	}

	LightComponent& LightComponent::withAmbient(float ambientCoefficient)
	{
		_light->withAmbient(ambientCoefficient);
		return *this;
	}

	LightComponent& LightComponent::withConeAngle(float angle)
	{
		_light->withConeAngle(angle);
		return *this;
	}

	void LightComponent::cleanContext()
	{
		if (_ctx != nullptr && _light != nullptr)
		{
			_ctx->getLighting().remove(_light);
		}
	}

	void LightComponent::onAssignedToContext(Context& ctx)
	{
		cleanContext();
		ctx.getLighting().add(_light);
		_ctx = &ctx;
	}

	void LightComponent::onAddedToEntity(Entity& entity)
	{
		onEntityTransformChanged(entity);
	}

	void LightComponent::onEntityParentTransformChanged(Entity& entity, Entity& parent)
	{
		onEntityTransformChanged(entity);
	}

	void LightComponent::onEntityTransformChanged(Entity& entity)
	{
		auto mat = entity.getModelMatrix();
		auto pos = glm::vec3(mat*glm::vec4(0, 0, 0, 1));
		_light->withPosition(pos);

		auto normal = glm::transpose(glm::inverse(glm::mat3(mat)));
		auto dir = normal*_direction;
		_light->withDirection(dir);
	}

}
