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
}

namespace mugato
{
	class Context;

	enum class LightType
	{
		Directional,
		Point
	};

	struct Light
	{
		typedef LightType Type;
		typedef std::map<std::string, gorn::UniformValue> UniformValueMap;

		Type type;
		glm::vec3 position;
		glm::vec3 color;
		float attenuation;
		float ambientCoefficient;
		float coneAngle;
		glm::vec3 direction;

		Light(Type type=Type::Point);


		UniformValueMap getUniforms() const;
	};

	class LightingSystem
	{
	private:
		typedef std::map<std::string, gorn::UniformValue> UniformValueMap;

		std::vector<std::shared_ptr<Light>> _lights;
		UniformValueMap getUniforms() const;
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