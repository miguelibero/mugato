#ifndef __mugato__ParticleSystemComponent__
#define __mugato__ParticleSystemComponent__

#include <mugato/scene/Component.hpp>
#include <mugato/sprite/ParticleSystem.hpp>
#include <string>

namespace mugato
{
    class ParticleSystemComponent : public Component
    {
    private:
        std::string _name;
        ParticleSystem _system;
    public:
		ParticleSystemComponent(const std::string& name);

		ParticleSystem& getParticleSystem();
        const ParticleSystem& getParticleSystem() const;

        void onAssignedToContext(Context& ctx) override;
        void onAddedToEntity(Entity& entity) override;
        void onEntityTransformChanged(Entity& entity) override;
        void update(double dt) override;
        void render(gorn::RenderQueue& queue) override;
    };
}

#endif
