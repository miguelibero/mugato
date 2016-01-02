
#include <mugato/spine/SpineSkeletonComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/scene/EntityTransform.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/spine/SpineManager.hpp>

namespace mugato
{
    SpineSkeletonComponent::SpineSkeletonComponent(const std::string& name):
    _name(name)
    {
    }

    SpineSkeleton& SpineSkeletonComponent::getSkeleton()
    {
        return _skeleton;
    }

    const SpineSkeleton& SpineSkeletonComponent::getSkeleton() const
    {
        return _skeleton;
    }

    void SpineSkeletonComponent::onAssignedToContext(Context& ctx)
    {
        _skeleton = ctx.getSkeletons().load(_name);
    }

    void SpineSkeletonComponent::onAddedToEntity(Entity& entity)
    {
        _entity = entity.getSharedPtr();
    }

    void SpineSkeletonComponent::update(double dt)
    {
        _skeleton.update(dt);
    }

    void SpineSkeletonComponent::render(gorn::RenderQueue& queue)
    {
        _skeleton.render(queue);
    }

}
