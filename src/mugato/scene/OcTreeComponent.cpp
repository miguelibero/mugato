#include <mugato/scene/OcTreeComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <gorn/render/RenderCommand.hpp>
#include <gorn/render/RenderKinds.hpp>
#include <gorn/asset/ShapeMeshFactory.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace mugato
{
    OcTreeComponent::OcTreeComponent():
    _elementsDrawMode(DrawMode::Lines),
    _nodesDrawMode(DrawMode::Lines),
    _needsAdjust(false)
    {
    }

    void OcTreeComponent::setElementsMaterial(
        const std::shared_ptr<gorn::Material>& mat)
    {
        _elementsMaterial = mat;
    }

    void OcTreeComponent::setNodesMaterial(
        const std::shared_ptr<gorn::Material>& mat)
    {
        _nodesMaterial = mat;
    }

    void OcTreeComponent::setElementsDrawMode(DrawMode mode)
    {
        _elementsMesh.setDrawMode(mode);
    }

    void OcTreeComponent::setNodesDrawMode(DrawMode mode)
    {
        _nodesMesh.setDrawMode(mode);
    }

    void OcTreeComponent::onAddedToEntity(Entity& entity)
    {
        onEntityTransformChanged(entity);
    }

    void OcTreeComponent::onEntityTransformChanged(Entity& entity)
    {
        _octree.setArea(gorn::Rect(glm::vec3(0.0f),
            entity.getTransform().getSize()));
    }

    void OcTreeComponent::onEntityChildAdded(Entity& entity,
        Entity& child)
    {
        onEntityChildTransformChanged(entity, child);
    }

    void OcTreeComponent::onEntityChildRemoved(Entity& entity,
        Entity& child)
    {
        _octree.clear([&child](const OcElement& elm){
            return elm.getContent() == child.getSharedPtr();
        });
    }

    void OcTreeComponent::onEntityChildTransformChanged(Entity& entity,
        Entity& child)
    {
        _octree.insert(OcElement(child.getTransform().getArea(), child.getSharedPtr()));
        _needsAdjust = true;
    }

    void OcTreeComponent::update(double dt)
    {
        if(_needsAdjust)
        {
            _octree.adjust(false);
            _needsAdjust = false;
        }
        _elementsMesh = gorn::ShapeMeshFactory::create(
                _octree.getElementsRects(), _elementsDrawMode);
        _nodesMesh = gorn::ShapeMeshFactory::create(
               _octree.getNodesRects(), _nodesDrawMode);
    }

    void OcTreeComponent::render(gorn::RenderQueue& queue)
    {
        if(!_elementsMesh.empty() || !_nodesMesh.empty())
        {
            queue.addCommand()
                .withTransform(glm::mat4(),
                    gorn::RenderTransformStackAction::PushLocal);
            queue.addCommand(_elementsMesh.render())
                .withMaterial(_elementsMaterial);
            queue.addCommand(_nodesMesh.render())
                .withMaterial(_nodesMaterial);
            queue.addCommand()
                .withTransformAction(
                    gorn::RenderTransformStackAction::PopLocal);
        }
    }
}
