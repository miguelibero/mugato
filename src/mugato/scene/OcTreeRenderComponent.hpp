#ifndef __mugato__OcTreeRenderComponent__
#define __mugato__OcTreeRenderComponent__

#include <mugato/scene/Component.hpp>
#include <mugato/sprite/Sprite.hpp>
#include <gorn/gl/Enums.hpp>
#include <gorn/gl/Material.hpp>
#include <gorn/asset/Mesh.hpp>
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
        gorn::Mesh _elementsMesh;
        gorn::Mesh _nodesMesh;
    public:
        OcTreeRenderComponent();

        void setElementsMaterial(const std::shared_ptr<gorn::Material>& mat);
        void setNodesMaterial(const std::shared_ptr<gorn::Material>& mat);
        void setElementsDrawMode(DrawMode mode);
        void setNodesDrawMode(DrawMode mode);

        void onAddedToEntity(Entity& entity) override;

        void update(double dt) override;
        void render(gorn::RenderQueue& queue) override;
    };
}

#endif
