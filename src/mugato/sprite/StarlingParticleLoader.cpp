
#include <mugato/sprite/StarlingParticleLoader.hpp>
#include <mugato/sprite/ParticleSystemConfig.hpp>
#include <gorn/base/String.hpp>
#include <gorn/gl/Enums.hpp>
#include <rapidxml/rapidxml.hpp>
#include <buffer.hpp>

using namespace rapidxml;

namespace mugato
{
	StarlingParticleLoader::StarlingParticleLoader()
    {
    }

	template<typename T>
	T StarlingParticleLoadAttribute(const xml_node<>* node, T default, const char* name = "value")
	{
		auto attr = node->first_attribute(name);
		if(attr == nullptr)
		{
			return default;
		}
		return gorn::String::convertTo<T>(attr->value());
	}

	glm::vec3 StarlingParticleLoadVector(const xml_node<>* node)
	{
		return glm::vec3(
			StarlingParticleLoadAttribute<float>(node, 0.0f, "x"),
			StarlingParticleLoadAttribute<float>(node, 0.0f, "y"),
			StarlingParticleLoadAttribute<float>(node, 0.0f, "z")
		);
	}

	glm::vec4 StarlingParticleLoadColor(const xml_node<>* node)
	{
		return glm::vec4(
			StarlingParticleLoadAttribute<float>(node, 0.0f, "red"),
			StarlingParticleLoadAttribute<float>(node, 0.0f, "green"),
			StarlingParticleLoadAttribute<float>(node, 0.0f, "blue"),
			StarlingParticleLoadAttribute<float>(node, 1.0f, "alpha")
		);
	}

	gorn::BlendFactor StarlingParticleGetBlendFactor(int type, gorn::BlendFactor default)
	{
		switch (type)
		{
		case 0:     return gorn::BlendFactor::Zero;
		case 1:     return gorn::BlendFactor::One;
		case 0x300: return gorn::BlendFactor::SrcColor;
		case 0x301: return gorn::BlendFactor::OneMinusSrcColor;
		case 0x302: return gorn::BlendFactor::SrcAlpha;
		case 0x303: return gorn::BlendFactor::OneMinusSrcAlpha;
		case 0x304: return gorn::BlendFactor::DstAlpha;;
		case 0x305: return gorn::BlendFactor::OneMinusDstAlpha;
		case 0x306: return gorn::BlendFactor::DstColor;
		case 0x307: return gorn::BlendFactor::OneMinusDstColor;
		default:    return default;
		}
	}

	gorn::BlendFactor StarlingParticleLoadBlendFactor(const xml_node<>* node, gorn::BlendFactor default)
	{
		return StarlingParticleGetBlendFactor(StarlingParticleLoadAttribute<int>(
			node, -1), default);
	}

	ParticleEmitterMode StarlingParticleLoadEmitterMode(const xml_node<>* node)
	{
		auto type = StarlingParticleLoadAttribute<int>(node, -1);
		switch (type)
		{
			case 0: return ParticleEmitterMode::Gravity;
			case 1: return ParticleEmitterMode::Radius;
			default: return ParticleEmitterMode::Normal;
		}
	}
    
    bool StarlingParticleLoader::validate(const buffer& data) const NOEXCEPT
    {
        if(data.binary())
        {
            return false;
        }
        xml_document<> doc;
		doc.parse<0>(const_cast<char*>(data.c_str()));
		auto lala = doc.first_node()->name();
        return std::string(doc.first_node()->name()) == "particleEmitterConfig";
    }

    ParticleSystemConfig StarlingParticleLoader::load(const buffer& data) const
    {
        xml_document<> doc;
		doc.parse<0>(const_cast<char*>(data.c_str()));
        
		ParticleSystemConfig config;
        auto root = doc.first_node("particleEmitterConfig");
		auto texture = root->first_node("texture");
		config.withSprite(texture->first_attribute("name")->value());
		auto& props = config.getProperties();

		props.withSourcePosition(StarlingParticleLoadVector(
			root->first_node("sourcePosition")));
		props.withSourcePositionVariance(StarlingParticleLoadVector(
			root->first_node("sourcePositionVariance")));
		props.withSpeed(StarlingParticleLoadAttribute<float>(
			root->first_node("speed"), 0.0f));
		props.withSpeedVariance(StarlingParticleLoadAttribute<float>(
			root->first_node("speedVariance"), 0.0f));
		props.withLife(StarlingParticleLoadAttribute<float>(
			root->first_node("particleLifeSpan"), 0.0f));
		props.withLifeVariance(StarlingParticleLoadAttribute<float>(
			root->first_node("particleLifespanVariance"), 0.0f));
		props.withAngle(glm::radians(360.0f - StarlingParticleLoadAttribute<float>(
			root->first_node("angle"), 0.0f)));
		props.withAngleVariance(glm::radians(StarlingParticleLoadAttribute<float>(
			root->first_node("angleVariance"), 0.0f)));
		props.withGravity(StarlingParticleLoadVector(
			root->first_node("gravity")));
		props.withRadialAcceleration(StarlingParticleLoadAttribute<float>(
			root->first_node("radialAcceleration"), 0.0f));
		props.withTangentialAcceleration(StarlingParticleLoadAttribute<float>(
			root->first_node("tangentialAcceleration"), 0.0f));
		props.withRadialAccelerationVariance(StarlingParticleLoadAttribute<float>(
			root->first_node("radialAccelVariance"), 0.0f));
		props.withTangentialAccelerationVariance(StarlingParticleLoadAttribute<float>(
			root->first_node("tangentialAccelVariance"), 0.0f));
		props.withStartColor(StarlingParticleLoadColor(
			root->first_node("startColor")));
		props.withStartColorVariance(StarlingParticleLoadColor(
			root->first_node("startColorVariance")));
		props.withEndColor(StarlingParticleLoadColor(
			root->first_node("finishColor")));
		props.withEndColorVariance(StarlingParticleLoadColor(
			root->first_node("finishColorVariance")));
		props.withTotalCount(StarlingParticleLoadAttribute<int>(
			root->first_node("maxParticles"), 0));
		props.withStartSize(StarlingParticleLoadAttribute<float>(
			root->first_node("startParticleSize"), 0.0f));
		props.withStartSizeVariance(StarlingParticleLoadAttribute<float>(
			root->first_node("startParticleSizeVariance"), 0.0f));
		props.withEndSize(StarlingParticleLoadAttribute<float>(
			root->first_node("finishParticleSize"), 0.0f));
		props.withEndSizeVariance(StarlingParticleLoadAttribute<float>(
			root->first_node("FinishParticleSizeVariance"), 0.0f));
		props.withDuration(StarlingParticleLoadAttribute<float>(
			root->first_node("duration"), 0.0f));
		props.withMode(StarlingParticleLoadEmitterMode(
			root->first_node("emitterType")));
		props.withStartRadius(StarlingParticleLoadAttribute<float>(
			root->first_node("maxRadius"), 0.0f));
		props.withStartRadiusVariance(StarlingParticleLoadAttribute<float>(
			root->first_node("maxRadiusVariance"), 0.0f));
		props.withEndRadius(StarlingParticleLoadAttribute<float>(
			root->first_node("minRadius"), 0.0f));
		props.withEndRadiusVariance(StarlingParticleLoadAttribute<float>(
			root->first_node("minRadiusVariance"), 0.0f));
		props.withRotatePerSecond(glm::radians(StarlingParticleLoadAttribute<float>(
			root->first_node("rotatePerSecond"), 0.0f)));
		props.withRotatePerSecondVariance(glm::radians(StarlingParticleLoadAttribute<float>(
			root->first_node("rotatePerSecondVariance"), 0.0f)));
		props.withStartSpin(glm::radians(StarlingParticleLoadAttribute<float>(
			root->first_node("rotationStart"), 0.0f)));
		props.withStartSpinVariance(glm::radians(StarlingParticleLoadAttribute<float>(
			root->first_node("rotationStartVariance"), 0.0f)));
		props.withEndSpin(glm::radians(StarlingParticleLoadAttribute<float>(
			root->first_node("rotationEnd"), 0.0f)));
		props.withEndSpinVariance(glm::radians(StarlingParticleLoadAttribute<float>(
			root->first_node("rotationEndVariance"), 0.0f)));
		props.withBlendMode(gorn::BlendMode(
			StarlingParticleLoadBlendFactor(
				root->first_node("blendFuncSource"), gorn::BlendFactor::SrcAlpha),
			StarlingParticleLoadBlendFactor(
				root->first_node("blendFuncDestination"), gorn::BlendFactor::OneMinusSrcAlpha)));

        return config;
    }

}
