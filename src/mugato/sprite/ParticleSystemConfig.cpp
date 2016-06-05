
#include <mugato/sprite/ParticleSystemConfig.hpp>

namespace mugato
{
	ParticleSystemConfig::ParticleSystemConfig()
	{
    }

	ParticleSystemConfig& ParticleSystemConfig::withSprite(const std::string& sprite)
	{
		_sprite = sprite;
		return *this;
	}

	ParticleSystemConfig& ParticleSystemConfig::withProperties(const Properties& properties)
	{
		_properties = properties;
		return *this;
	}

	const std::string& ParticleSystemConfig::getSprite() const
	{
		return _sprite;
	}

	const ParticleSystemConfig::Properties& ParticleSystemConfig::getProperties() const
	{
		return _properties;
	}

	ParticleSystemConfig::Properties& ParticleSystemConfig::getProperties()
	{
		return _properties;
	}

}
