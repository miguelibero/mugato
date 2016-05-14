

#include <mugato/sprite/ParticleManager.hpp>
#include <mugato/sprite/SpriteManager.hpp>
#include <mugato/sprite/Sprite.hpp>
#include <mugato/sprite/ParticleSystem.hpp>

namespace mugato {

    ParticleManager::ParticleManager(mugato::SpriteManager& sprites, gorn::FileManager& files):
    _sprites(sprites)
    {
    }

    const ParticleManager::Definitions& ParticleManager::getDefinitions() const
    {
        return _definitions;
    }

	ParticleManager::Definitions& ParticleManager::getDefinitions()
    {
        return _definitions;
    }

    ParticleSystem ParticleManager::load(const std::string& dname)
    {
		ParticleSystem particles;
        auto& def = getDefinitions().get(dname);
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
