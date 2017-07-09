
#include <mugato/scene/ModelComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/scene/EntityTransform.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/model/ModelManager.hpp>

namespace mugato
{
	ModelComponent::ModelComponent(const std::string& name):
    _name(name)
    {
    }

    Model& ModelComponent::getModel()
    {
        return _model;
    }

    const Model& ModelComponent::getModel() const
    {
        return _model;
    }

    void ModelComponent::onAssignedToContext(Context& ctx)
	{
		_model = ctx.getModels().load(_name);
    }

    void ModelComponent::onAddedToEntity(Entity& entity)
    {
        onEntityTransformChanged(entity);
    }

    void ModelComponent::onEntityTransformChanged(Entity& entity)
    {

    }

    void ModelComponent::update(double dt)
    {
		_model.update(dt);
    }

    void ModelComponent::render(gorn::RenderQueue& queue)
    {
		_model.render(queue);
    }

}
