
#include <mugato/sprite/ParticleSystem.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <gorn/render/RenderCommand.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace mugato
{
	Particle::Particle(const Sprite& sprite, const Properties& props):
	_sprite(sprite), _currentTime(0.0), _totalTime(0.0), _props(&props),
	_emitRadius(0.0f), _emitRadiusDelta(0.0f), _emitRotation(0.0f),
	_emitRotationDelta(0.0f), _radialAcceleration(0.0f), _tangentialAcceleration(0.0f),
	_size(0.0f), _sizeDelta(0.0f), _spin(0.0f), _spinDelta(0.0f)
	{
		init();
	}

	bool Particle::finished() const
	{
		return _currentTime >= _totalTime;
	}

	void Particle::init()
	{
		_sprite.setResizeMode(mugato::SpriteResizeMode::Exact);
		auto life = _props->getRandomLife();
		_totalTime = life > 0.0f ? life : 0.0;
		if(_totalTime <= 0.0)
		{
			return;
		}

		_position = _props->getRandomSourcePosition();
		auto angle = _props->getRandomAngle();
		// TODO: fix for 3D
		_velocity = _props->getRandomSpeed() * glm::vec3(
			std::cos(angle), std::sin(angle), 0.0f
		);

		auto startRadius = _props->getRandomStartRadius();
		auto endRadius = _props->getRandomEndRadius();

		_emitRadius = startRadius;
		_emitRadiusDelta = (endRadius - startRadius) / life;
		_emitRotation = _props->getRandomAngle();
		_emitRotationDelta = _props->getRandomRotatePerSecond();
		_radialAcceleration = _props->getRandomRadialAcceleration();
		_tangentialAcceleration = _props->getRandomTangentialAcceleration();

		auto startSize = _props->getRandomStartSize();
		auto endSize = _props->getRandomEndSize();
		_size = startSize;
		_sizeDelta = (endSize - startSize) / life;

		auto startSpin = _props->getRandomStartSpin();
		auto endSpin = _props->getRandomEndSpin();
		_spin = startSpin;
		_spinDelta = (endSpin - startSpin) / life;

		auto startColor = _props->getRandomStartColor();
		auto endColor = _props->getRandomEndColor();

		_color = startColor;
		_colorDelta = (endColor - startColor) / life;
	}

	void Particle::update(double dt)
	{
		auto restTime = _totalTime - _currentTime;
		dt = restTime > dt ? dt : restTime;
		_currentTime += dt;

		auto fdt = (float)dt;

		if(_props->getMode() == ParticleEmitterMode::Radius)
		{
			_emitRotation += _emitRotationDelta * fdt;
			_emitRadius += _emitRadiusDelta * fdt;

			_position = _props->getSourcePosition() - glm::vec3(
				std::cos(_emitRotation), std::sin(_emitRotation), 0.0f
			) * _emitRadius;
		}
		else
		{
			auto delta = _position - _props->getSourcePosition();
			auto distance = glm::length(delta);
			if(distance < 0.01f)
			{
				distance = 0.01f;
			}
			auto radial = delta / distance;
			auto tangential = radial;
			radial *= _radialAcceleration;
			tangential = glm::vec3(tangential.y, tangential.x, 0.0f) * _tangentialAcceleration;

			_velocity += (_props->getGravity() + radial + tangential) * fdt;
			_position += _velocity * fdt;
		}

		_size += _sizeDelta * fdt;
		_spin += _spinDelta * fdt;

		_sprite.setSize(glm::vec2(_size, _size));

		_sprite.update(dt);
	}

	void Particle::render(gorn::RenderQueue& queue) const
	{
		queue.addCommand()
			.withTransform(glm::translate(glm::mat4(), _position-glm::vec3(_size, _size, 0.0f)*0.5f))
			.withTransformAction(gorn::RenderTransformStackAction::PushLocal);
		auto cmd = _sprite.render();
		cmd.withRepeatAttribute(gorn::AttributeType::Color,
			buffer{ _color.r, _color.g, _color.b, _color.a });
		queue.addCommand(std::move(cmd));
		queue.addCommand()
			.withTransformAction(gorn::RenderTransformStackAction::PopLocal);
	}

	ParticleSystem::ParticleSystem() :
	_frameTime(0.0)
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
		_particles.erase(std::remove_if(_particles.begin(), _particles.end(), [](const Particle& p){
			return p.finished();
		}), _particles.end());

		for(auto& particle : _particles)
		{
			particle.update(dt);
		}

		auto timeBetweenParticles = 1.0f / _properties.getEmissionRate();
		_frameTime += dt;

		while(_frameTime > 0.0)
		{
			if(_particles.size() < _properties.getTotalCount())
			{
				_particles.push_back(Particle(_sprite, _properties));
				auto& particle = _particles.back();
				particle.update(_frameTime);
			}
			_frameTime -= timeBetweenParticles;
		}
	}

	void ParticleSystem::render(gorn::RenderQueue& queue) const
	{
		queue.addCommand()
			.withBlendMode(_properties.getBlendMode());
		for(auto& particle : _particles)
		{
			particle.render(queue);
		}
		queue.addCommand()
			.withBlendModeAction(gorn::RenderStackAction::Pop);
	}
}
