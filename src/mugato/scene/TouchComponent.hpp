#ifndef __mugato__TouchComponent__
#define __mugato__TouchComponent__

#include <mugato/scene/Component.hpp>
#include <gorn/asset/Mesh.hpp>
#include <functional>

namespace mugato
{
    class TouchComponent : public Component
    {
    public:
        typedef std::function<bool(Entity& entity, const glm::vec3& p,
            TouchPhase phase)> Callback;
    private:
        Callback _callback;
		gorn::Mesh _rayMesh;
		std::shared_ptr<gorn::Material> _rayMaterial;
    public:
        TouchComponent(const Callback& cb);
        bool onScreenTouched(Entity& entity, const gorn::RenderCamera& cam,
            const glm::vec2& p, TouchPhase phase) override;
    };
}

#endif
