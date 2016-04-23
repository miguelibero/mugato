#ifndef __mugato__AlignComponent__
#define __mugato__AlignComponent__

#include <mugato/scene/Component.hpp>
#include <mugato/base/Alignment.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace mugato
{
    class AlignComponent : public Component
    {
    public:
        typedef std::vector<glm::length_t> Constraints;
    private:
        std::weak_ptr<Entity> _entity;
        Alignment _align;
        bool _dirty;
        Constraints _constraints;
    public:
        AlignComponent(const Alignment& align);
        AlignComponent(const Alignment& align,
        const Constraints& constraints);

        void onAddedToEntity(Entity& entity) override;
        void onEntityParentTransformChanged(Entity& entity, Entity& parent) override;
        void onEntityTransformChanged(Entity& entity) override;
        void update(double dt) override;
    };
}

#endif
