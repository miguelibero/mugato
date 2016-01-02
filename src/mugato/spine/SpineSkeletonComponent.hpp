#ifndef __mugato__SpineSkeletonComponent__
#define __mugato__SpineSkeletonComponent__

#include <mugato/scene/Component.hpp>
#include <mugato/spine/SpineSkeleton.hpp>
#include <string>
#include <glm/glm.hpp>

namespace mugato
{
    class SpineSkeletonComponent : public Component
    {
    private:
        std::string _name;
        std::string _animation;
        SpineSkeleton _skeleton;
        std::weak_ptr<Entity> _entity;
    public:
        SpineSkeletonComponent(const std::string& name);

        SpineSkeleton& getSkeleton();
        const SpineSkeleton& getSkeleton() const;

        void onAssignedToContext(Context& ctx) override;
        void onAddedToEntity(Entity& entity) override;
        void update(double dt) override;
        void render(gorn::RenderQueue& queue) override;
    };
}

#endif
