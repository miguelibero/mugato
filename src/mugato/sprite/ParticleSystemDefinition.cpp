
#include <mugato/sprite/ParticleSystemDefinition.hpp>

namespace mugato
{

	ParticleSystemDefinition::ParticleSystemDefinition()
	{
    }

	ParticleSystemDefinition& ParticleSystemDefinition::withConfig(const std::string& name)
	{
		_configName = name;
		return *this;
	}

	ParticleSystemDefinition& ParticleSystemDefinition::withConfig(Config&& config)
	{
		return withConfig(std::make_shared<Config>(std::move(config)));
	}

	ParticleSystemDefinition& ParticleSystemDefinition::withConfig(const std::shared_ptr<Config>& config)
	{
		_config = config;
		return *this;
	}

	std::shared_ptr<ParticleSystemDefinition::Config> ParticleSystemDefinition::getConfig() const
	{
		return _config;
	}

	const std::string& ParticleSystemDefinition::getConfigName() const
	{
		return _configName;
	}

}
