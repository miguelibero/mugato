

#include <mugato/sprite/ParticleManager.hpp>
#include <mugato/sprite/SpriteManager.hpp>
#include <mugato/sprite/Sprite.hpp>
#include <mugato/sprite/ParticleSystem.hpp>

namespace mugato {

    ParticleManager::ParticleManager(mugato::SpriteManager& sprites, gorn::FileManager& files):
    _sprites(sprites),
	_configs(files)
    {
		getDefinitions().set([](const std::string& name) {
			return Definition()
				.withConfig(name);
		});
    }

    const ParticleManager::Definitions& ParticleManager::getDefinitions() const
    {
        return _definitions;
    }

	ParticleManager::Definitions& ParticleManager::getDefinitions()
    {
        return _definitions;
    }

	const ParticleManager::Configs& ParticleManager::getConfigs() const
	{
		return _configs;
	}

	ParticleManager::Configs& ParticleManager::getConfigs()
	{
		return _configs;
	}

    ParticleSystem ParticleManager::load(const std::string& dname)
    {
		ParticleSystem particles;
        auto& def = getDefinitions().get(dname);
		auto config = def.getConfig();
		if(config == nullptr)
		{
			config = getConfigs().load(def.getConfigName()).get();
		}
		if(config != nullptr)
		{
			particles.setSprite(_sprites.load(config->getSprite()));
			particles.setProperties(config->getProperties());
		}
        return particles;
    }

	ParticleSystem ParticleManager::tryLoad(const std::string& name)
    {
        if(!getDefinitions().has(name))
        {
            return ParticleSystem();
        }
        return load(name);
    }

}
