#include <mugato/scene/RenderInfoComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/base/Context.hpp>
#include <gorn/base/Context.hpp>

namespace mugato {

    RenderInfoComponent::RenderInfoComponent(const std::string& font):
    _font(font)
    {
        _label.setAlignment(Alignment::BottomLeft);
    }

    RenderInfoComponent::Transform& RenderInfoComponent::getTransform()
    {
        return _transform;
    }

    const RenderInfoComponent::Transform& RenderInfoComponent::
        getTransform() const
    {
        return _transform;
    }

    void RenderInfoComponent::onAssignedToContext(Context& ctx)
    {
        _label.setFont(ctx.getLabels().loadLabelFont(_font));
    }

    void RenderInfoComponent::onAddedToEntity(Entity& entity)
    {
        _entity = entity.getSharedPtr();
        onEntityTransformChanged(entity);
    }

    void RenderInfoComponent::onEntityTransformChanged(Entity& entity)
    {
        _label.setSize(glm::vec2(entity.getTransform().getSize()));
    }

    void RenderInfoComponent::update(double dt)
    {
        if(auto ptr = _entity.lock())
        {
            auto& info = ptr->getContext().getGorn().getQueue().getInfo();
            _label.setText(info.str());
        }
        _label.update(dt);
        _transform.update();
    }

    void RenderInfoComponent::render(gorn::RenderQueue& queue)
    {
        queue.addCommand()
            .withTransformMode(
                gorn::RenderCommand::TransformMode::PushCheckpoint);
        queue.addCommand()
            .withTransform(_transform.getMatrix());
        _label.render(queue);
        queue.addCommand()
            .withTransformMode(
                gorn::RenderCommand::TransformMode::PopCheckpoint);
    }

}
