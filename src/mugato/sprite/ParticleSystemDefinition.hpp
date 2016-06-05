

#ifndef __mugato__ParticleSystemDefinition__
#define __mugato__ParticleSystemDefinition__

#include <memory>
#include <mugato/sprite/ParticleSystemConfig.hpp>

namespace mugato {

    class ParticleSystemDefinition
    {
	public:
		typedef ParticleSystemConfig Config;
    private:
		std::shared_ptr<Config> _config;
		std::string _configName;

    public:
		ParticleSystemDefinition();

		ParticleSystemDefinition& withConfig(const std::string& name);
		ParticleSystemDefinition& withConfig(Config&& config);
		ParticleSystemDefinition& withConfig(const std::shared_ptr<Config>& config);

		std::shared_ptr<Config> getConfig() const;
		const std::string& getConfigName() const;
    };
}

#endif
