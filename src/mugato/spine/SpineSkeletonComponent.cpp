
#include <mugato/spine/SpineSkeletonComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/scene/EntityTransform.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/spine/SpineManager.hpp>

namespace mugato
{
    SpineSkeletonComponent::SpineSkeletonComponent(const std::string& name):
    _name(name),
    _pivotPercent(0.0f)
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

    void SpineSkeletonComponent::setAnimation(const std::string& name)
    {
        _skeleton.play(_animation);
        _animation = name;
    }

    void SpineSkeletonComponent::setEntityPivotPercent(const glm::vec2& val)
    {
        _pivotPercent = val;
        if(auto ptr = _entity.lock())
        {
            ptr->getTransform().setPivot(_pivotPercent*_skeleton.getSize());
        }
    }

    void SpineSkeletonComponent::setEntitySize()
    {
        if(auto ptr = _entity.lock())
        {
            ptr->getTransform().setSize(_skeleton.getSize());
        }
    }

    void SpineSkeletonComponent::onAddedToEntity(Entity& entity)
    {
        _skeleton = entity.getContext().getSkeletons().load(_name);
        _entity = entity.getSharedPtr();
        setEntityPivotPercent(_pivotPercent);
        setEntitySize();
        _skeleton.play(_animation);
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
