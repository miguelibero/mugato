#ifndef __mugato__ParticleSystem__
#define __mugato__ParticleSystem__

#include <glm/glm.hpp>
#include <vector>
#include <mugato/sprite/Sprite.hpp>
#include <mugato/sprite/ParticleSystemProperties.hpp>

namespace gorn
{
	class RenderQueue;
}

namespace mugato
{
	class Particle
	{
	public:
		typedef ParticleSystemProperties Properties;
	private:
		Sprite _sprite;
		double _currentTime;
		double _totalTime;
		const Properties* _props;
		glm::vec3 _position;
		glm::vec3 _velocity;
		float _emitRadius;
		float _emitRadiusDelta;
		float _emitRotation;
		float _emitRotationDelta;
		float _radialAcceleration;
		float _tangentialAcceleration;
		float _size;
		float _sizeDelta;
		float _spin;
		float _spinDelta;
		glm::vec4 _color;
		glm::vec4 _colorDelta;

		void init();
	public:
		Particle(const Sprite& sprite, const Properties& props);
		bool finished() const;
		void update(double dt);
		void render(gorn::RenderQueue& queue) const;
	};


    class ParticleSystem
    {
	public:
		typedef ParticleSystemProperties Properties;
    private:
		Properties _properties;
		glm::vec3 _size;
		Sprite _sprite;
		std::vector<Particle> _particles;
		double _frameTime;
    public:
		ParticleSystem();

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
