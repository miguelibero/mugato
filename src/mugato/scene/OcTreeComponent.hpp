#ifndef __mugato__OcTreeComponent__
#define __mugato__OcTreeComponent__

#include <mugato/scene/Component.hpp>
#include <mugato/sprite/Sprite.hpp>
#include <mugato/base/OcTree.hpp>
#include <gorn/gl/Enums.hpp>
#include <gorn/gl/Material.hpp>
#include <gorn/asset/Mesh.hpp>
#include <string>
#include <memory>

namespace mugato
{
    class OcTreeComponent : public Component
    {
    public:
        typedef gorn::DrawMode DrawMode;
        typedef OcTree<std::shared_ptr<Entity>> EntityOcTree;
        typedef EntityOcTree::Element OcElement;
        typedef EntityOcTree::Elements OcElements;
    private:
        std::shared_ptr<gorn::Material> _elementsMaterial;
        std::shared_ptr<gorn::Material> _nodesMaterial;
        gorn::DrawMode _elementsDrawMode;
        gorn::DrawMode _nodesDrawMode;
        gorn::Mesh _elementsMesh;
        gorn::Mesh _nodesMesh;
        EntityOcTree _octree;
        bool _needsAdjust;
    public:
        OcTreeComponent();

        void setElementsMaterial(const std::shared_ptr<gorn::Material>& mat);
        void setNodesMaterial(const std::shared_ptr<gorn::Material>& mat);
        void setElementsDrawMode(DrawMode mode);
        void setNodesDrawMode(DrawMode mode);

        void onAddedToEntity(Entity& entity) override;
        void onEntityTransformChanged(Entity& entity) override;
        void onEntityChildAdded(Entity& entity,
            const std::shared_ptr<Entity>& child) override;
        void onEntityChildRemoved(Entity& entity,
            const std::shared_ptr<Entity>& child) override;
        void onEntityChildTransformChanged(Entity& entity,
            const std::shared_ptr<Entity>& child) override;
        void update(double dt) override;
        void render(gorn::RenderQueue& queue) override;
    };
}

#endif
