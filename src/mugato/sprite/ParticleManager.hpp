

#ifndef __mugato__ParticleManager__
#define __mugato__ParticleManager__

#include <gorn/asset/AssetManager.hpp>
#include <gorn/base/DefinitionManager.hpp>
#include <mugato/sprite/ParticleSystemDefinition.hpp>


namespace mugato
{
	class SpriteManager;
    class ParticleSystem;
	class ParticleSystemConfig;

    class ParticleManager
    {
    public:
        typedef ParticleSystemDefinition Definition;
		typedef ParticleSystemConfig Config;
        typedef gorn::DefinitionManager<Definition> Definitions;
		typedef gorn::AssetManager<Config> Configs;
    private:
        mugato::SpriteManager& _sprites;
        Definitions _definitions;
		Configs _configs;
    public:
		ParticleManager(mugato::SpriteManager& sprites, gorn::FileManager& files);
		ParticleManager(const ParticleManager& other) = delete;

        const Definitions& getDefinitions() const;
        Definitions& getDefinitions();
		const Configs& getConfigs() const;
		Configs& getConfigs();

        ParticleSystem load(const std::string& name);
		ParticleSystem tryLoad(const std::string& name);
    };
}

#endif
