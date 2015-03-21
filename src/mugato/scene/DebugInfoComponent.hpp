#ifndef __mugato__DebugInfoComponent__
#define __mugato__DebugInfoComponent__

#include <mugato/scene/Component.hpp>
#include <mugato/label/Label.hpp>
#include <mugato/scene/EntityTransform.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <string>

namespace mugato
{
    class DebugInfoComponent : public Component
    {
    public:
        typedef gorn::RenderQueue::DebugInfo Info;
        typedef EntityTransform Transform;
    private:
        Transform _transform;
        std::weak_ptr<Entity> _entity;
        Label _label;
        std::string _font;
        Info _info;
        
    public:
        DebugInfoComponent(const std::string& font="debug:");

        Transform& getTransform();
        const Transform& getTransform() const;

        void onAddedToEntity(Entity& entity) override;
        bool update(double dt) override;
        void render(gorn::RenderQueue& queue) override;
    };
}

#endif
