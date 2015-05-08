#include <mugato/scene/TouchComponent.hpp>

namespace mugato
{
    TouchComponent::TouchComponent(const Callback& cb):
    _callback(cb)
    {
    }

    void TouchComponent::onEntityTouched(Entity& entity, const glm::vec2& p)
    {
        if(_callback)
        {
            _callback(entity, p);
        }
    }
}
