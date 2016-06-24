

#ifndef __mugato__ParticleSystemProperties__
#define __mugato__ParticleSystemProperties__

#include <glm/glm.hpp>
#include <gorn/gl/BlendMode.hpp>

namespace mugato {

	enum ParticleEmitterMode
	{
		Normal,
		Gravity,
		Radius
	};

    class ParticleSystemProperties
    {
	public:
		typedef ParticleEmitterMode Mode;
    private:
		glm::vec3 _gravity;
		float _speed;
		float _speedVar;
		float _tanAccel;
		float _tanAccelVar;
		float _radAccel;
		float _radAccelVar;
		bool _rotIsDir;
		float _startRadius;
		float _startRadiusVar;
		float _endRadius;
		float _endRadiusVar;
		float _rotPerSec;
		float _rotPerSecVar;
		float _duration;
		glm::vec3 _srcPos;
		glm::vec3 _srcPosVar;
		float _life;
		float _lifeVar;
		float _angle;
		float _angleVar;
		Mode _mode;
		float _startSize;
		float _startSizeVar;
		float _endSize;
		float _endSizeVar;
		glm::vec4 _startColor;
		glm::vec4 _startColorVar;
		glm::vec4 _endColor;
		glm::vec4 _endColorVar;
		float _startSpin;
		float _startSpinVar;
		float _endSpin;
		float _endSpinVar;
		unsigned int _totalCount;
		gorn::BlendMode _blendMode;

    public:
		ParticleSystemProperties();

		ParticleSystemProperties& withGravity(const glm::vec3& gravity);
		ParticleSystemProperties& withSpeed(float speed);
		ParticleSystemProperties& withSpeedVariance(float var);
		ParticleSystemProperties& withTangentialAcceleration(float accel);
		ParticleSystemProperties& withTangentialAccelerationVariance(float var);
		ParticleSystemProperties& withRadialAcceleration(float rad);
		ParticleSystemProperties& withRadialAccelerationVariance(float var);
		ParticleSystemProperties& withRotationIsDir(bool is);
		ParticleSystemProperties& withStartRadius(float rad);
		ParticleSystemProperties& withStartRadiusVariance(float var);
		ParticleSystemProperties& withEndRadius(float rad);
		ParticleSystemProperties& withEndRadiusVariance(float var);
		ParticleSystemProperties& withRotatePerSecond(float rot);
		ParticleSystemProperties& withRotatePerSecondVariance(float var);
		ParticleSystemProperties& withDuration(float duration);
		ParticleSystemProperties& withSourcePosition(const glm::vec3& pos);
		ParticleSystemProperties& withSourcePositionVariance(const glm::vec3& var);
		ParticleSystemProperties& withLife(float life);
		ParticleSystemProperties& withLifeVariance(float var);
		ParticleSystemProperties& withAngle(float angle);
		ParticleSystemProperties& withAngleVariance(float var);
		ParticleSystemProperties& withMode(Mode mode);
		ParticleSystemProperties& withStartSize(float size);
		ParticleSystemProperties& withStartSizeVariance(float var);
		ParticleSystemProperties& withEndSize(float size);
		ParticleSystemProperties& withEndSizeVariance(float var);
		ParticleSystemProperties& withStartColor(const glm::vec4& color);
		ParticleSystemProperties& withStartColorVariance(const glm::vec4& var);
		ParticleSystemProperties& withEndColor(const glm::vec4& color);
		ParticleSystemProperties& withEndColorVariance(const glm::vec4& var);
		ParticleSystemProperties& withStartSpin(float spin);
		ParticleSystemProperties& withStartSpinVariance(float var);
		ParticleSystemProperties& withEndSpin(float spin);
		ParticleSystemProperties& withEndSpinVariance(float var);
		ParticleSystemProperties& withTotalCount(int count);
		ParticleSystemProperties& withBlendMode(const gorn::BlendMode& mode);

		const glm::vec3& getGravity() const;
		float getSpeed() const;
		float getSpeedVariance() const;
		float getTangentialAcceleration() const;
		float getTangentialAccelerationVariance() const;
		float getRadialAcceleration() const;
		float getRadialAccelerationVariance() const;
		bool getRotationIsDir() const;
		float getStartRadius() const;
		float getStartRadiusVariance() const;
		float getEndRadius() const;
		float getEndRadiusVariance() const;
		float getRotatePerSecond() const;
		float getRotatePerSecondVariance() const;
		float getDuration() const;
		const glm::vec3& getSourcePosition() const;
		const glm::vec3& getSourcePositionVariance() const;
		float getLife() const;
		float getLifeVariance() const;
		float getAngle() const;
		float getAngleVariance() const;
		Mode getMode() const;
		float getStartSize() const;
		float getStartSizeVariance() const;
		float getEndSize() const;
		float getEndSizeVariance() const;
		const glm::vec4& getStartColor() const;
		const glm::vec4& getStartColorVariance() const;
		const glm::vec4& getEndColor() const;
		const glm::vec4& getEndColorVariance() const;
		float getStartSpin() const;
		float getStartSpinVariance() const;
		float getEndSpin() const;
		float getEndSpinVariance() const;
		float getEmissionRate() const;
		unsigned int getTotalCount() const;
		const gorn::BlendMode& getBlendMode() const;

		float getRandomSpeed() const;
		float getRandomTangentialAcceleration() const;
		float getRandomRadialAcceleration() const;
		float getRandomStartRadius() const;
		float getRandomEndRadius() const;
		float getRandomRotatePerSecond() const;
		glm::vec3 getRandomSourcePosition() const;
		float getRandomLife() const;
		float getRandomAngle() const;
		float getRandomStartSize() const;
		float getRandomEndSize() const;
		glm::vec4 getRandomStartColor() const;
		glm::vec4 getRandomEndColor() const;
		float getRandomStartSpin() const;
		float getRandomEndSpin() const;
    };
}

#endif
