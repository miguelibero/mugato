#include <mugato/scene/OcTreeRenderComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/base/Rectangle.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <gorn/render/RenderCommand.hpp>
#include <gorn/render/Kinds.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
            size_t c = children.getArea().flat()?2:3;

            queue.addCommand()
                .withTransform(glm::translate(glm::mat4(),
                    glm::vec3(0.0f, 0.0f, 1.0f)),
                    gorn::RenderCommand::TransformMode::PushLocal);
            queue.addCommand()
                .withMaterial(_elementsMaterial)
                .withDrawMode(_elementsDrawMode)
                .withAttribute(gorn::AttributeKind::Position,
                    children.getElementsVertices(_elementsDrawMode),
                    c, gorn::BasicType::Float);
            queue.addCommand()
                .withMaterial(_nodesMaterial)
                .withDrawMode(_nodesDrawMode)
                .withAttribute(gorn::AttributeKind::Position,
                    children.getNodesVertices(_nodesDrawMode),
                    c, gorn::BasicType::Float);
            queue.addCommand()
                .withTransformMode(
                    gorn::RenderCommand::TransformMode::PopLocal);

        }
    }
}

