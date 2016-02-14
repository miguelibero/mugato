#include <mugato/base/Light.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/base/Kinds.hpp>
#include <gorn/gl/ProgramDefinition.hpp>
#include <gorn/gl/VertexArray.hpp>
#include <gorn/render/RenderKinds.hpp>
#include <sstream>

namespace mugato
{
	LightingSystem::LightingSystem()
	{
	}

	// shaders taken from https://github.com/tomdalling/opengl-series

	void LightingSystem::setup(mugato::Context& ctx)
	{
		ctx.getGorn().getPrograms().getDefinitions().get(ProgramKind::Light)
			.withShaderData(gorn::ShaderType::Vertex, R"(#version 150

uniform mat4 camera;
uniform mat4 model;

in vec3 vert;
in vec2 vertTexCoord;
in vec3 vertNormal;

out vec3 fragVert;
out vec2 fragTexCoord;
out vec3 fragNormal;

void main() {
    // Pass some variables to the fragment shader
    fragTexCoord = vertTexCoord;
    fragNormal = vertNormal;
    fragVert = vert;
    
    // Apply all matrix transformations to vert
    gl_Position = camera * model * vec4(vert, 1);
}

)")
.withShaderData(gorn::ShaderType::Fragment, R"(#version 150

	uniform mat4 model;
uniform vec3 cameraPosition;

uniform sampler2D materialTex;
uniform float materialShininess;
uniform vec3 materialSpecularColor;

#define MAX_LIGHTS 10
uniform int numLights;
uniform struct Light {
   vec4 position;
   vec3 intensities; //a.k.a the color of the light
   float attenuation;
   float ambientCoefficient;
   float coneAngle;
   vec3 coneDirection;
} allLights[MAX_LIGHTS];

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragVert;

out vec4 finalColor;

vec3 ApplyLight(Light light, vec3 surfaceColor, vec3 normal, vec3 surfacePos, vec3 surfaceToCamera) {
    vec3 surfaceToLight;
    float attenuation = 1.0;

		if(light.position.w == 0.0) {
        //directional light
        surfaceToLight = normalize(light.position.xyz);
        attenuation = 1.0; //no attenuation for directional lights
    } else {
        //point light
        surfaceToLight = normalize(light.position.xyz - surfacePos);
        float distanceToLight = length(light.position.xyz - surfacePos);
        attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

			//cone restrictions (affects attenuation)
        float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(light.coneDirection))));
        if(lightToSurfaceAngle > light.coneAngle){
            attenuation = 0.0;
        }
    }

		//ambient
	vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.intensities;

		//diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.intensities;
    
    //specular
    float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0)
	{
        specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
	}
    vec3 specular = specularCoefficient * materialSpecularColor * light.intensities;

	//linear color (color before gamma correction)
    return ambient + attenuation*(diffuse + specular);
}

void main() {
    vec3 normal = normalize(transpose(inverse(mat3(model))) * fragNormal);
    vec3 surfacePos = vec3(model * vec4(fragVert, 1));
    vec4 surfaceColor = texture(materialTex, fragTexCoord);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);

	//combine color from all the lights
    vec3 linearColor = vec3(0);
    for(int i = 0; i < numLights; ++i){
        linearColor += ApplyLight(allLights[i], surfaceColor.rgb, normal, surfacePos, surfaceToCamera);
    }
    
    //final color (after gamma correction)
    vec3 gamma = vec3(1.0/2.2);
    finalColor = vec4(pow(linearColor, gamma), surfaceColor.a);
}

)")
		.withUniform(gorn::UniformKind::Model, "model")
		.withUniform(gorn::UniformKind::CameraPosition, "cameraPosition")
		.withUniform(gorn::UniformKind::Camera, "camera")
		.withUniform(gorn::UniformKind::Texture0, "materialTex")
		.withUniform(gorn::UniformKind::Color,
			gorn::ProgramUniformDefinition("materialSpecularColor")
				.withValue(glm::vec3(1.0)))
		.withUniform(gorn::ProgramUniformDefinition("materialShininess").withValue(1.0f))
		.withAttribute(gorn::AttributeKind::TexCoords, "vertTexCoord")
		.withAttribute(gorn::AttributeKind::Normal, "vertNormal")
		.withAttribute(gorn::AttributeKind::Position, "vert");
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
		light.type = type;
		return light;
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

	LightingSystem::UniformValueMap LightingSystem::getUniforms() const
	{
		int i = 0;
		std::stringstream ss;
		UniformValueMap uniforms;
		for(auto& light : _lights)
		{
			ss << "allLights[" << i++ << "].";
			std::string prefix = ss.str();
			ss.str("");
			auto lightUniforms = light->getUniforms();
			for(auto itr = lightUniforms.begin(); itr != lightUniforms.end(); ++itr)
			{
				uniforms[prefix+itr->first] = itr->second;
			}
		}
		uniforms["numLights"] = (int)_lights.size();
		return uniforms;
	}

	void LightingSystem::render(gorn::RenderQueue& queue) const
	{
		// TODO: check for light changes to not reload the uniforms all the time
		auto uniforms = queue.getUniformValues();
		for(auto itr = uniforms.begin(); itr != uniforms.end(); ++itr)
		{
			if (itr->first.substr(0, 10) == "allLights[")
			{
				queue.removeUniformValue(itr->first);
			}
		}
		queue.removeUniformValue("numLights");
		uniforms = getUniforms();
		for (auto itr = uniforms.begin(); itr != uniforms.end(); ++itr)
		{
			queue.setUniformValue(itr->first, itr->second);
		}
	}

	Light::Light(Type type):
	type(type),
	position(0.0f),
	color(1.0f),
	attenuation(0.0f),
	ambientCoefficient(1.0f),
	coneAngle(glm::pi<float>()),
	direction(-1.0f, -1.0f, 1.0f)
	{
	}

	Light::UniformValueMap Light::getUniforms() const
	{
		UniformValueMap uniforms;
		uniforms["position"] = glm::vec4(position, (int)type);
		uniforms["intensities"] = color;
		uniforms["attenuation"] = attenuation;
		uniforms["ambientCoefficient"] = ambientCoefficient;
		uniforms["coneAngle"] = coneAngle;
		uniforms["coneDirection"] = direction;
		return uniforms;
	}

}