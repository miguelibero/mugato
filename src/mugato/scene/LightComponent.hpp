#ifndef __mugato__LightComponent__
#define __mugato__LightComponent__

#include <mugato/scene/Component.hpp>
#include <mugato/base/Light.hpp>
#include <memory>

namespace mugato
{
	class Light;

    class LightComponent : public Component
    {
	public:
		typedef Light::Type Type;
    private:
		std::shared_ptr<Light> _light;
		glm::vec3 _direction;
		Context* _ctx;

		void cleanContext();
    public:
		LightComponent();
		~LightComponent();

		LightComponent& withType(Type type);
		LightComponent& withColor(const glm::vec3& color);
		LightComponent& withDirection(const glm::vec3& dir);
		LightComponent& withAttenuation(float attenuation);
		LightComponent& withAmbient(float ambientCoefficient);
		LightComponent& withConeAngle(float angle);

		void onAssignedToContext(Context& ctx) override;
		void onAddedToEntity(Entity& entity) override;
		void onEntityParentTransformChanged(Entity& entity, Entity& parent) override;
        void onEntityTransformChanged(Entity& entity) override;


    };
}

#endif
