
#include <mugato/sprite/ParticleSystem.hpp>

namespace mugato
{
	ParticleSystem::ParticleSystem()
	{
	}

	ParticleSystem::ParticleSystem(const Sprite& sprite):
	_sprite(sprite)
	{
	}

	const glm::vec3& ParticleSystem::getSize() const
	{
		return _size;
	}

	void ParticleSystem::setSize(const glm::vec3& size)
	{
		_size = size;
	}

	void ParticleSystem::setProperties(const Properties& props)
	{
		_properties = props;
	}

	const ParticleSystem::Properties& ParticleSystem::getProperties() const
	{
		return _properties;
	}

	ParticleSystem::Properties& ParticleSystem::getProperties()
	{
		return _properties;
	}

	void ParticleSystem::setSprite(const Sprite& sprite)
	{
		_sprite = sprite;
	}

	const Sprite& ParticleSystem::getSprite() const
	{
		return _sprite;
	}
	
	Sprite& ParticleSystem::getSprite()
	{
		return _sprite;
	}

	void ParticleSystem::update(double dt)
	{
	}

	void ParticleSystem::render(gorn::RenderQueue& queue) const
	{

	}
}
