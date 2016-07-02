#include <mugato/base/Light.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/base/Kinds.hpp>
#include <gorn/gl/ProgramDefinition.hpp>
#include <gorn/gl/VertexArray.hpp>
#include <gorn/render/RenderKinds.hpp>
#include <sstream>
#include <algorithm>

namespace mugato
{
	LightingSystem::LightingSystem()
	{
	}

	// shaders taken from https://github.com/tomdalling/opengl-series

	void LightingSystem::setup(mugato::Context& ctx)
	{
		ctx.getGorn().getPrograms().getDefinitions().get(ProgramKind::Light)
			.withShaderData(gorn::ShaderType::Vertex, R"(#version 100
precision highp float;

uniform mat4 camera;
uniform mat4 model;

attribute vec3 vert;
attribute vec2 vertTexCoord;
attribute vec3 vertNormal;

varying vec3 fragVert;
varying vec2 fragTexCoord;
varying vec3 fragNormal;

void main() {
    fragTexCoord = vertTexCoord;
    fragNormal = vertNormal;
    fragVert = vert;
    gl_Position = camera * model * vec4(vert, 1);
}

)")
.withShaderData(gorn::ShaderType::Fragment, R"(#version 100
precision highp float;

uniform mat4 model;
uniform mat3 normalMatrix;
uniform vec3 cameraPosition;

uniform sampler2D materialTex;
uniform float materialShininess;
uniform vec3 materialSpecularColor;

#define MAX_LIGHTS 10
uniform int numLights;
uniform struct Light {
   vec4 position;
   vec3 intensities;
   float attenuation;
   float ambientCoefficient;
   float coneAngle;
   vec3 coneDirection;
} allLights[MAX_LIGHTS];

varying vec2 fragTexCoord;
varying vec3 fragNormal;
varying vec3 fragVert;

vec3 ApplyLight(Light light, vec3 surfaceColor, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera) {
    vec3 surfaceToLight;
    float attenuation = 1.0;
    if(light.position.w == 0.0) {
        surfaceToLight = normalize(light.position.xyz);
    } else {
        surfaceToLight = normalize(light.position.xyz - surfacePos);
        float distanceToLight = distance(light.position.xyz, surfacePos);
        attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2.0));

        float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(light.coneDirection))));
        if(lightToSurfaceAngle > light.coneAngle) {
            attenuation = 0.0;
        }
    }

    vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.intensities;

    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.intensities;

    float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0) {
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
    }
    vec3 specular = specularCoefficient * materialSpecularColor * light.intensities;

    return ambient + attenuation*(diffuse + specular);
}

void main() {
    vec3 normal = normalize(normalMatrix * fragNormal);
    vec3 surfacePos = vec3(model * vec4(fragVert, 1));
    vec4 surfaceColor = texture2D(materialTex, fragTexCoord);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);

    vec3 linearColor = vec3(0);
    for(int i = 0; i < numLights; ++i) {
        linearColor += ApplyLight(allLights[i], surfaceColor.rgb, normal, surfacePos, surfaceToCamera);
    }

    vec3 gamma = vec3(1.0/2.2);
    gl_FragColor = vec4(pow(linearColor, gamma), surfaceColor.a);
}

)")
		.withUniform(gorn::UniformKind::Model, "model")
		.withUniform(gorn::UniformKind::NormalMatrix, "normalMatrix")
		.withUniform(gorn::UniformKind::CameraPosition, "cameraPosition")
		.withUniform(gorn::UniformKind::Camera, "camera")
		.withUniform(gorn::UniformKind::Texture0, "materialTex")
		.withUniform(gorn::UniformKind::Color,
			gorn::ProgramUniformDefinition("materialSpecularColor")
				.withDefaultValue(glm::vec3(1.0)))
		.withUniform(
			gorn::ProgramUniformDefinition("materialShininess")
				.withDefaultValue(80.0f))
		.withAttribute(gorn::AttributeKind::TexCoords,
			gorn::ProgramAttributeDefinition("vertTexCoord")
				.withType(gorn::BasicType::Float)
				.withCount(2))
		.withAttribute(gorn::AttributeKind::Normal,
			gorn::ProgramAttributeDefinition("vertNormal")
				.withType(gorn::BasicType::Float)
				.withCount(3))
		.withAttribute(gorn::AttributeKind::Position,
			gorn::ProgramAttributeDefinition("vert")
				.withType(gorn::BasicType::Float)
				.withCount(3));
	}

	Light& LightingSystem::add(const std::shared_ptr<Light>& light)
	{
		if(light == nullptr)
		{
			return add();
		}
		_lights.push_back(light);
		return *light;
	}

	Light& LightingSystem::add(LightType type)
	{
		auto& light = add();
		return light.withType(type);
	}

	Light& LightingSystem::add()
	{
		return add(std::make_shared<Light>());
	}

	bool LightingSystem::remove(const std::shared_ptr<Light>& light)
	{
		auto itr = std::remove(_lights.begin(), _lights.end(), light);
		if(itr == _lights.end())
		{
			return false;
		}
		_lights.erase(itr, _lights.end());
		return true;
	}

	gorn::UniformValueMap LightingSystem::getUniformValues(const gorn::RenderCamera& cam) const
	{
		int i = 0;
		std::stringstream ss;
		gorn::UniformValueMap values;
		for(auto& light : _lights)
		{
			if(cam.matchesLayers(light->getLayers()))
			{
				ss << "allLights[" << i++ << "].";
				std::string prefix = ss.str();
				ss.str("");
				auto lightUniforms = light->getUniformValues();
				for (auto itr = lightUniforms.begin(); itr != lightUniforms.end(); ++itr)
				{
					values[prefix + itr->first] = itr->second;
				}
			}
		}
		values["numLights"] = i;
		return values;
	}

	void LightingSystem::render(gorn::RenderQueue& queue) const
	{
		for(auto& cam : queue.getCameras())
		{
			cam->withUniformValues(getUniformValues(*cam));
		}
	}

	Light::Light(Type type):
	_type(type),
	_position(0.0f),
	_color(1.0f),
	_attenuation(0.0f),
	_ambientCoefficient(0.0f),
	_coneAngle(90.0f),
	_direction(0, -1, 0)
	{
	}

	Light& Light::withType(Type type)
	{
		_type = type;
		return *this;
	}

	Light& Light::withPosition(const glm::vec3& position)
	{
		_position = position;
		return *this;
	}

	Light& Light::withColor(const glm::vec3& color)
	{
		_color = color;
		return *this;
	}

	Light& Light::withDirection(const glm::vec3& dir)
	{
		_direction = dir;
		return *this;
	}

	Light& Light::withAttenuation(float attenuation)
	{
		_attenuation = attenuation;
		return *this;
	}

	Light& Light::withAmbient(float ambientCoefficient)
	{
		_ambientCoefficient = ambientCoefficient;
		return *this;
	}

	Light& Light::withConeAngle(float angle)
	{
		_coneAngle = angle;
		return *this;
	}

	Light& Light::withLayer(int layer)
	{
		_layers.push_back(layer);
		return *this;
	}

	Light& Light::withLayers(const Layers& layers)
	{
		_layers.insert(_layers.end(), layers.begin(), layers.end());
		return *this;
	}

	const Light::Layers& Light::getLayers() const
	{
		return _layers;
	}

	gorn::UniformValueMap Light::getUniformValues() const
	{
		return{
			{ "position",			glm::vec4(_position, (int)_type) },
			{ "intensities",		_color },
			{ "attenuation",		_attenuation },
			{ "ambientCoefficient", _ambientCoefficient },
			{ "coneAngle",			_coneAngle },
			{ "coneDirection",		_direction },
		};
	}

}
