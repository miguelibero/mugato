
#include <mugato/sprite/ParticleSystemProperties.hpp>
#include <gorn/base/Math.hpp>

namespace mugato
{
	ParticleSystemProperties::ParticleSystemProperties():
	_gravity(0.0f, -10.0f, 0.0f),
	_speed(0.0f),
	_speedVar(0.0f),
	_tanAccel(0.0f),
	_tanAccelVar(0.0f),
	_radAccel(0.0f),
	_radAccelVar(0.0f),
	_rotIsDir(false),
	_startRadius(0.0f),
	_startRadiusVar(0.0f),
	_endRadius(0.0f),
	_endRadiusVar(0.0f),
	_rotPerSec(0.0f),
	_rotPerSecVar(0.0f),
	_duration(0.0f),
	_srcPos(0.0f, 0.0f, 0.0f),
	_srcPosVar(0.0f, 0.0f, 0.0f),
	_life(0.0f),
	_lifeVar(0.0f),
	_angle(0.0f),
	_angleVar(0.0f),
	_mode(Mode::Normal),
	_startSize(0.0f),
	_startSizeVar(0.0f),
	_endSize(0.0f),
	_endSizeVar(0.0f),
	_startColor(1.0f, 1.0f, 1.0f, 1.0f),
	_startColorVar(0.0f, 0.0f, 0.0f, 0.0f),
	_endColor(1.0f, 1.0f, 1.0f, 1.0f),
	_endColorVar(0.0f, 0.0f, 0.0f, 0.0f),
	_startSpin(0.0f),
	_startSpinVar(0.0f),
	_endSpin(0.0f),
	_endSpinVar(0.0f),
	_totalCount(0)
	{
    }

	ParticleSystemProperties& ParticleSystemProperties::withGravity(const glm::vec3& gravity)
	{
		_gravity = gravity;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withSpeed(float speed)
	{
		_speed = speed;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withSpeedVariance(float var)
	{
		_speedVar = var;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withTangentialAcceleration(float accel)
	{
		_tanAccel = accel;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withTangentialAccelerationVariance(float var)
	{
		_tanAccelVar = var;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withRadialAcceleration(float rad)
	{
		_radAccel = rad;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withRadialAccelerationVariance(float var)
	{
		_radAccelVar = var;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withRotationIsDir(bool is)
	{
		_rotIsDir = is;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withStartRadius(float rad)
	{
		_startRadius = rad;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withStartRadiusVariance(float var)
	{
		_startRadiusVar = var;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withEndRadius(float rad)
	{
		_endRadius = rad;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withEndRadiusVariance(float var)
	{
		_endRadiusVar = var;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withRotatePerSecond(float rot)
	{
		_rotPerSec = rot;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withRotatePerSecondVariance(float var)
	{
		_rotPerSecVar = var;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withDuration(float duration)
	{
		_duration = duration;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withSourcePosition(const glm::vec3& pos)
	{
		_srcPos = pos;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withSourcePositionVariance(const glm::vec3& var)
	{
		_srcPosVar = var;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withLife(float life)
	{
		_life = life;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withLifeVariance(float var)
	{
		_lifeVar = var;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withAngle(float angle)
	{
		_angle = angle;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withAngleVariance(float var)
	{
		_angleVar = var;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withMode(Mode mode)
	{
		_mode = mode;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withStartSize(float size)
	{
		_startSize = size;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withStartSizeVariance(float var)
	{
		_startSizeVar = var;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withEndSize(float size)
	{
		_endSize = size;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withEndSizeVariance(float var)
	{
		_endSizeVar = var;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withStartColor(const glm::vec4& color)
	{
		_startColor = color;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withStartColorVariance(const glm::vec4& var)
	{
		_startColorVar = var;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withEndColor(const glm::vec4& color)
	{
		_endColor = color;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withEndColorVariance(const glm::vec4& var)
	{
		_endColorVar = var;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withStartSpin(float spin)
	{
		_startSpin = spin;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withStartSpinVariance(float var)
	{
		_startSpinVar = var;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withEndSpin(float spin)
	{
		_endSpin = spin;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withEndSpinVariance(float var)
	{
		_endSpinVar = var;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withTotalCount(int count)
	{
		_totalCount = count;
		return *this;
	}

	ParticleSystemProperties& ParticleSystemProperties::withBlendMode(const gorn::BlendMode& mode)
	{
		_blendMode = mode;
		return *this;
	}

	const glm::vec3& ParticleSystemProperties::getGravity() const
	{
		return _gravity;
	}

	float ParticleSystemProperties::getSpeed() const
	{
		return _speed;
	}
	float ParticleSystemProperties::getSpeedVariance() const
	{
		return _speedVar;
	}

	float ParticleSystemProperties::getTangentialAcceleration() const
	{
		return _tanAccel;
	}

	float ParticleSystemProperties::getTangentialAccelerationVariance() const
	{
		return _tanAccelVar;
	}

	float ParticleSystemProperties::getRadialAcceleration() const
	{
		return _radAccel;
	}

	float ParticleSystemProperties::getRadialAccelerationVariance() const
	{
		return _radAccelVar;
	}

	bool ParticleSystemProperties::getRotationIsDir() const
	{
		return _rotIsDir;
	}

	float ParticleSystemProperties::getStartRadius() const
	{
		return _startRadius;
	}

	float ParticleSystemProperties::getStartRadiusVariance() const
	{
		return _startRadiusVar;
	}

	float ParticleSystemProperties::getEndRadius() const
	{
		return _endRadius;
	}

	float ParticleSystemProperties::getEndRadiusVariance() const
	{
		return _endRadiusVar;
	}

	float ParticleSystemProperties::getRotatePerSecond() const
	{
		return _rotPerSec;
	}

	float ParticleSystemProperties::getRotatePerSecondVariance() const
	{
		return _rotPerSecVar;
	}

	float ParticleSystemProperties::getDuration() const
	{
		return _duration;
	}

	const glm::vec3& ParticleSystemProperties::getSourcePosition() const
	{
		return _srcPos;
	}

	const glm::vec3& ParticleSystemProperties::getSourcePositionVariance() const
	{
		return _srcPosVar;
	}

	float ParticleSystemProperties::getLife() const
	{
		return _life;
	}

	float ParticleSystemProperties::getLifeVariance() const
	{
		return _lifeVar;
	}

	float ParticleSystemProperties::getAngle() const
	{
		return _angle;
	}

	float ParticleSystemProperties::getAngleVariance() const
	{
		return _angleVar;
	}

	ParticleSystemProperties::Mode ParticleSystemProperties::getMode() const
	{
		return _mode;
	}

	float ParticleSystemProperties::getStartSize() const
	{
		return _startSize;
	}

	float ParticleSystemProperties::getStartSizeVariance() const
	{
		return _startSizeVar;
	}

	float ParticleSystemProperties::getEndSize() const
	{
		return _endSize;
	}

	float ParticleSystemProperties::getEndSizeVariance() const
	{
		return _endSizeVar;
	}

	const glm::vec4& ParticleSystemProperties::getStartColor() const
	{
		return _startColor;
	}

	const glm::vec4& ParticleSystemProperties::getStartColorVariance() const
	{
		return _startColorVar;
	}

	const glm::vec4& ParticleSystemProperties::getEndColor() const
	{
		return _endColor;
	}

	const glm::vec4& ParticleSystemProperties::getEndColorVariance() const
	{
		return _endColorVar;
	}

	float ParticleSystemProperties::getStartSpin() const
	{
		return _startSpin;
	}

	float ParticleSystemProperties::getStartSpinVariance() const
	{
		return _startSpinVar;
	}

	float ParticleSystemProperties::getEndSpin() const
	{
		return _endSpin;
	}

	float ParticleSystemProperties::getEndSpinVariance() const
	{
		return _endSpinVar;
	}

	float ParticleSystemProperties::getEmissionRate() const
	{
		return (float)_totalCount / _life;
	}

	unsigned int ParticleSystemProperties::getTotalCount() const
	{
		return _totalCount;
	}

	const gorn::BlendMode& ParticleSystemProperties::getBlendMode() const
	{
		return _blendMode;
	}

	float ParticleSystemProperties::getRandomSpeed() const
	{
		return _speed + gorn::Math::random(-1.0f, 1.0f) * _speedVar;
	}

	float ParticleSystemProperties::getRandomTangentialAcceleration() const
	{
		return _tanAccel + gorn::Math::random(-1.0f, 1.0f) * _tanAccelVar;
	}

	float ParticleSystemProperties::getRandomRadialAcceleration() const
	{
		return _radAccel + gorn::Math::random(-1.0f, 1.0f) * _radAccelVar;
	}

	float ParticleSystemProperties::getRandomStartRadius() const
	{
		return _startRadius + gorn::Math::random(-1.0f, 1.0f) * _startRadiusVar;
	}
	float ParticleSystemProperties::getRandomEndRadius() const
	{
		return _endRadius + gorn::Math::random(-1.0f, 1.0f) * _endRadiusVar;
	}

	float ParticleSystemProperties::getRandomRotatePerSecond() const
	{
		return _rotPerSec + gorn::Math::random(-1.0f, 1.0f) * _rotPerSecVar;
	}

	glm::vec3 ParticleSystemProperties::getRandomSourcePosition() const
	{
		return _srcPos + gorn::Math::random(-1.0f, 1.0f) * _srcPosVar;
	}

	float ParticleSystemProperties::getRandomLife() const
	{
		return _life + gorn::Math::random(-1.0f, 1.0f) * _lifeVar;
	}

	float ParticleSystemProperties::getRandomAngle() const
	{
		return _angle + gorn::Math::random(-1.0f, 1.0f) * _angleVar;
	}

	float ParticleSystemProperties::getRandomStartSize() const
	{
		return _startSize + gorn::Math::random(-1.0f, 1.0f) * _startSizeVar;
	}

	float ParticleSystemProperties::getRandomEndSize() const
	{
		return _endSize + gorn::Math::random(-1.0f, 1.0f) * _endSizeVar;
	}

	glm::vec4 ParticleSystemProperties::getRandomStartColor() const
	{
		return _startColor + gorn::Math::random(-1.0f, 1.0f) * _startColorVar;
	}

	glm::vec4 ParticleSystemProperties::getRandomEndColor() const
	{
		return _endColor + gorn::Math::random(-1.0f, 1.0f) * _endColorVar;
	}
	
	float ParticleSystemProperties::getRandomStartSpin() const
	{
		return _startSpin + gorn::Math::random(-1.0f, 1.0f) * _startSpinVar;
	}

	float ParticleSystemProperties::getRandomEndSpin() const
	{
		return _endSpin + gorn::Math::random(-1.0f, 1.0f) * _endSpinVar;
	}
}
