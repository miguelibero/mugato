#include <mugato/scene/OcTreeRenderComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/base/Rectangle.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <gorn/render/RenderCommand.hpp>
#include <gorn/render/Kinds.hpp>

namespace mugato
{
    OcTreeRenderComponent::OcTreeRenderComponent():
    _elementsDrawMode(DrawMode::Lines),
    _nodesDrawMode(DrawMode::Lines)
    {
    }

    void OcTreeRenderComponent::setElementsMaterial(
        const std::shared_ptr<gorn::Material>& mat)
    {
        _elementsMaterial = mat;
    }

    void OcTreeRenderComponent::setNodesMaterial(
        const std::shared_ptr<gorn::Material>& mat)
    {
        _nodesMaterial = mat;
    }

    void OcTreeRenderComponent::setElementsDrawMode(DrawMode mode)
    {
        _elementsDrawMode = mode;
    }

    void OcTreeRenderComponent::setNodesDrawMode(DrawMode mode)
    {
        _nodesDrawMode = mode;
    }

    void OcTreeRenderComponent::onAddedToEntity(Entity& entity)
    {
        _entity = entity.getSharedPtr();
    }

    void OcTreeRenderComponent::update(double dt)
    {
    }

    void OcTreeRenderComponent::render(gorn::RenderQueue& queue)
    {
        if(auto ptr = _entity.lock())
        {
            auto& children = ptr->getChildren();
            size_t c = mugato::Rectangle::getVertexCount(_elementsDrawMode);
            queue.addCommand()
                .withMaterial(_elementsMaterial)
                .withAttribute(gorn::AttributeKind::Position,
                    children.getElementsVertices(_elementsDrawMode),
                    children.size()*c);
            c = mugato::Rectangle::getVertexCount(_nodesDrawMode);
            queue.addCommand()
                .withMaterial(_nodesMaterial)
                .withAttribute(gorn::AttributeKind::Position,
                    children.getNodesVertices(_nodesDrawMode),
                    children.sizeNodes()*c);
        }
    }
}

