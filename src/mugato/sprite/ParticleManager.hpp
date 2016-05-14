

#ifndef __mugato__ParticleManager__
#define __mugato__ParticleManager__

#include <gorn/asset/AssetManager.hpp>
#include <gorn/base/DefinitionManager.hpp>
#include <mugato/sprite/ParticleSystemDefinition.hpp>


namespace mugato
{
	class SpriteManager;
    class ParticleSystem;

    class ParticleManager
    {
    public:
        typedef ParticleSystemDefinition Definition;
        typedef gorn::DefinitionManager<Definition> Definitions;
    private:
        mugato::SpriteManager& _sprites;
        Definitions _definitions;
    public:
		ParticleManager(mugato::SpriteManager& sprites, gorn::FileManager& files);
		ParticleManager(const ParticleManager& other) = delete;

        const Definitions& getDefinitions() const;
        Definitions& getDefinitions();

        ParticleSystem load(const std::string& name);
		ParticleSystem tryLoad(const std::string& name);
    };
}

#endif
