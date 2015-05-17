#ifndef __mugato__TouchComponent__
#define __mugato__TouchComponent__

#include <mugato/scene/Component.hpp>
#include <functional>

namespace mugato
{
    class TouchComponent : public Component
    {
    public:
        typedef std::function<void(Entity& entity, const glm::vec2& p)> Callback;
    private:
        Callback _callback;
    public:
        TouchComponent(const Callback& cb);
        void onEntityTouched(Entity& entity, const glm::vec2& p) override;
    };
}

#endif
