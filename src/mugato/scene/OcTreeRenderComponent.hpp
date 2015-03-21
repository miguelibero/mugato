#ifndef __mugato__OcTreeRenderComponent__
#define __mugato__OcTreeRenderComponent__

#include <mugato/scene/Component.hpp>
#include <mugato/sprite/Sprite.hpp>
#include <gorn/render/Enums.hpp>
#include <gorn/render/Material.hpp>
#include <string>
#include <memory>

namespace mugato
{
    class OcTreeRenderComponent : public Component
    {
    public:
        typedef gorn::DrawMode DrawMode;
    private:
        std::weak_ptr<Entity> _entity;
        std::shared_ptr<gorn::Material> _elementsMaterial;
        std::shared_ptr<gorn::Material> _nodesMaterial;
        gorn::DrawMode _elementsDrawMode;
        gorn::DrawMode _nodesDrawMode;        

    public:
        OcTreeRenderComponent();

        void setElementsMaterial(const std::shared_ptr<gorn::Material>& mat);
        void setNodesMaterial(const std::shared_ptr<gorn::Material>& mat);
        void setElementsDrawMode(DrawMode mode);
        void setNodesDrawMode(DrawMode mode);

        void onAddedToEntity(Entity& entity) override;
        bool update(double dt) override;
        void render(gorn::RenderQueue& queue) override;
    };
}

#endif
