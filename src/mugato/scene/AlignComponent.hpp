#ifndef __mugato__AlignComponent__
#define __mugato__AlignComponent__

#include <mugato/scene/Component.hpp>
#include <mugato/base/Alignment.hpp>
#include <memory>

namespace mugato
{
    class AlignComponent : public Component
    {
    private:
        std::weak_ptr<Entity> _entity;
        Alignment _align;
        bool _dirty;
    public:
        AlignComponent(const Alignment& align);

        void onAddedToEntity(Entity& entity) override;
        void onParentSizeChanged(Entity& entity) override;
        void onEntityTransformChanged(Entity& entity) override;
        void update(double dt) override;
    };
}

#endif
