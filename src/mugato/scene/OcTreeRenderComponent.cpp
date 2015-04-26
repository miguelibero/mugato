#include <mugato/scene/OcTreeRenderComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <gorn/render/RenderCommand.hpp>
#include <gorn/render/RenderKinds.hpp>
#include <gorn/asset/ShapeMeshFactory.hpp>
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
        _elementsMesh.setDrawMode(mode);
    }

    void OcTreeRenderComponent::setNodesDrawMode(DrawMode mode)
    {
        _nodesMesh.setDrawMode(mode);
    }

    void OcTreeRenderComponent::onAddedToEntity(Entity& entity)
    {
        _entity = entity.getSharedPtr();
    }

    void OcTreeRenderComponent::update(double dt)
    {
        if(auto ptr = _entity.lock())
        {
            auto& children = ptr->getChildren();
            _elementsMesh.clear();
            auto rects = children.getElementsRects();
            for(auto& rect : rects)
            {
                _elementsMesh += gorn::ShapeMeshFactory::create(
                    rect, _elementsDrawMode);
            }
            _nodesMesh.clear();
            rects = children.getNodesRects();
            for(auto& rect : rects)
            {
                _nodesMesh += gorn::ShapeMeshFactory::create(
                    rect, _nodesDrawMode);
            }
        }
    }

    void OcTreeRenderComponent::render(gorn::RenderQueue& queue)
    {
        if(!_elementsMesh.empty() || !_nodesMesh.empty())
        {
            queue.addCommand()
                .withTransform(glm::translate(glm::mat4(),
                    glm::vec3(0.0f, 0.0f, 1.0f)),
                    gorn::RenderCommand::TransformMode::PushLocal);
            queue.addCommand(_elementsMesh.render())
                .withMaterial(_elementsMaterial);
            queue.addCommand(_nodesMesh.render())
                .withMaterial(_nodesMaterial);
            queue.addCommand()
                .withTransformMode(
                    gorn::RenderCommand::TransformMode::PopLocal);
        }
    }
}

