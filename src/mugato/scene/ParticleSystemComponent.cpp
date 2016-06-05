
#include <mugato/scene/ParticleSystemComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/scene/EntityTransform.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/sprite/ParticleManager.hpp>

namespace mugato
{
	ParticleSystemComponent::ParticleSystemComponent(const std::string& name):
    _name(name)
    {
    }

	ParticleSystem& ParticleSystemComponent::getParticleSystem()
    {
        return _system;
    }

    const ParticleSystem& ParticleSystemComponent::getParticleSystem() const
    {
        return _system;
    }

    void ParticleSystemComponent::onAssignedToContext(Context& ctx)
    {
		_system = ctx.getParticles().load(_name);
    }

    void ParticleSystemComponent::onAddedToEntity(Entity& entity)
    {
        onEntityTransformChanged(entity);
    }

    void ParticleSystemComponent::onEntityTransformChanged(Entity& entity)
    {
        auto& size = entity.getTransform().getSize();
		_system.setSize(size);
    }

    void ParticleSystemComponent::update(double dt)
    {
		_system.update(dt);
    }

    void ParticleSystemComponent::render(gorn::RenderQueue& queue)
    {
		_system.render(queue);
    }

}
