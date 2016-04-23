#include <mugato/scene/AlignComponent.hpp>
#include <mugato/scene/Entity.hpp>

namespace mugato
{
    AlignComponent::AlignComponent(const Alignment& align):
    _align(align), _dirty(false)
    {
    }

    AlignComponent::AlignComponent(const Alignment& align,
        const Constraints& constraints):
    _align(align), _dirty(false), _constraints(constraints)
    {
    }

    void AlignComponent::onAddedToEntity(Entity& entity)
    {
        _entity = entity.getSharedPtr();
        _dirty = true;
    }

    void AlignComponent::onEntityTransformChanged(Entity& entity)
    {
        _dirty = true;
    }

    void AlignComponent::onEntityParentTransformChanged(Entity& entity, Entity& parent)
    {
        _dirty = true;
    }

    void AlignComponent::update(double dt)
    {
        if(!_dirty)
        {
            return;
        }
        auto entity = _entity.lock();
        if(entity && entity->hasParent())
        {
            auto& pt = entity->getParent().getTransform();
            auto psize = pt.getSize()/pt.getScale();
            auto size = entity->getTransform().getSize()
                *entity->getTransform().getScale();
            auto apos = alignPosition(_align, psize, size);
            glm::vec3 pos = apos;
            if(!_constraints.empty())
            {
                pos = entity->getTransform().getPosition();
                for(auto& i : _constraints)
                {
                    pos[i] = apos[i];
                }
            }                
            entity->getTransform().setPosition(pos);
        }
        _dirty = false;
    }
}

