#include <mugato/scene/AlignComponent.hpp>
#include <mugato/scene/Entity.hpp>

namespace mugato
{
    AlignComponent::AlignComponent(const Alignment& align):
    _align(align), _dirty(false)
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

    void AlignComponent::onParentSizeChanged(Entity& entity)
    {
        _dirty = true;
    }

    void AlignComponent::update(double dt)
    {
        if(_dirty)
        {
            if(auto entity = _entity.lock())
            {
                if(entity->hasParent())
                {
                    auto& pt = entity->getParent().getTransform();
                    auto psize = pt.getSize()/pt.getScale();
                    auto& size = entity->getTransform().getSize();
                    auto pos = alignPosition(_align, psize, size);
                    entity->getTransform().setPosition(pos);
                }
            }
            _dirty = false;
        }
    }
}

