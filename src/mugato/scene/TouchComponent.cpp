#include <mugato/scene/TouchComponent.hpp>

namespace mugato
{
    TouchComponent::TouchComponent(const Callback& cb):
    _callback(cb)
    {
    }

    bool TouchComponent::onEntityTouched(Entity& entity,
        const glm::vec2& p, TouchPhase phase)
    {
        if(_callback)
        {
            return _callback(entity, p, phase);
        }
        return false;
    }
}
