#ifndef __mugato__TouchComponent__
#define __mugato__TouchComponent__

#include <mugato/scene/Component.hpp>
#include <functional>

namespace mugato
{
    class TouchComponent : public Component
    {
    public:
        typedef std::function<bool(Entity& entity, const glm::vec2& p,
            TouchPhase phase)> Callback;
    private:
        Callback _callback;
    public:
        TouchComponent(const Callback& cb);
        bool onEntityTouched(Entity& entity, const glm::vec2& p,
            TouchPhase phase) override;
    };
}

#endif
