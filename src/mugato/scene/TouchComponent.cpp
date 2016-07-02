#include <mugato/scene/TouchComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <gorn/render/RenderCamera.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <gorn/base/Ray.hpp>
#include <gorn/asset/ShapeMeshFactory.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace mugato
{
    TouchComponent::TouchComponent(const Callback& cb):
    _callback(cb)
    {
    }

    bool TouchComponent::onScreenTouched(Entity& entity,
        const gorn::RenderCamera& cam, const glm::vec2& p, TouchPhase phase)
    {
        if(_callback)
        {
			glm::vec3 hp;
			if(entity.hitBy(cam.getScreenPointRay(p), hp))
			{
				return _callback(entity, hp, phase);
			}
        }
        return false;
    }
}
