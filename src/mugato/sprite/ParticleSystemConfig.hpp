

#ifndef __mugato__ParticleSystemConfig__
#define __mugato__ParticleSystemConfig__

#include <mugato/sprite/ParticleSystemProperties.hpp>
#include <string>

namespace mugato {

    class ParticleSystemConfig
    {
	public:
		typedef ParticleSystemProperties Properties;
    private:
		std::string _sprite;
		Properties _properties;

    public:
		ParticleSystemConfig();

		ParticleSystemConfig& withSprite(const std::string& sprite);
		ParticleSystemConfig& withProperties(const Properties& properties);

		const std::string& getSprite() const;
		const Properties& getProperties() const;
		Properties& getProperties();
    };
}

#endif
