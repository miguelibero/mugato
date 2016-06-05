#ifndef __mugato__ParticleSystem__
#define __mugato__ParticleSystem__

#include <glm/glm.hpp>
#include <mugato/sprite/Sprite.hpp>
#include <mugato/sprite/ParticleSystemProperties.hpp>

namespace gorn
{
	class RenderQueue;
}

namespace mugato
{
    class ParticleSystem
    {
	public:
		typedef ParticleSystemProperties Properties;
    private:
		Properties _properties;
		glm::vec3 _size;
		Sprite _sprite;
    public:
		ParticleSystem();
		ParticleSystem(const Sprite& sprite);

		const glm::vec3& getSize() const;
		void setSize(const glm::vec3& size);

		void setProperties(const Properties& props);
		const Properties& getProperties() const;
		Properties& getProperties();

		void setSprite(const Sprite& sprite);
		const Sprite& getSprite() const;
		Sprite& getSprite();

		void update(double dt);
		void render(gorn::RenderQueue& queue) const;
    };

}

#endif
