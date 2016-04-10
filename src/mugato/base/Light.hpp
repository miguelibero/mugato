#ifndef __mugato__Light__
#define __mugato__Light__

#include <gorn/gl/UniformValue.hpp>
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <string>
#include <memory>

namespace gorn
{
	class RenderQueue;
	class RenderCamera;
}

namespace mugato
{
	class Context;

	enum class LightType
	{
		Directional,
		Point
	};

	class Light
	{
	public:
		typedef LightType Type;
		typedef std::vector<int> Layers;
	private:
		Type _type;
		glm::vec3 _position;
		glm::vec3 _color;
		float _attenuation;
		float _ambientCoefficient;
		float _coneAngle;
		glm::vec3 _direction;
		Layers _layers;
	public:
		Light(Type type=Type::Point);

		Light& withType(Type type);
		Light& withPosition(const glm::vec3& position);
		Light& withColor(const glm::vec3& color);
		Light& withDirection(const glm::vec3& dir);
		Light& withAttenuation(float attenuation);
		Light& withAmbient(float ambientCoefficient);
		Light& withConeAngle(float angle);
		Light& withLayer(int layer);
		Light& withLayers(const Layers& layers);

		gorn::UniformValueMap getUniformValues() const;
		const Layers& getLayers() const;
	};

	class LightingSystem
	{
	private:
		std::vector<std::shared_ptr<Light>> _lights;
		gorn::UniformValueMap getUniformValues(const gorn::RenderCamera& cam) const;
	public:

		LightingSystem();
		void setup(mugato::Context& ctx);
		Light& add(const std::shared_ptr<Light>& light);
		Light& add(LightType type);
		Light& add();
		bool remove(const std::shared_ptr<Light>& light);
		void render(gorn::RenderQueue& queue) const;
	};
}

#endif