#include <mugato/scene/DebugInfoComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/base/Context.hpp>
#include <gorn/base/Context.hpp>
#include <sstream>

namespace mugato {

    DebugInfoComponent::DebugInfoComponent(const std::string& font):
    _font(font)
    {
    }

    DebugInfoComponent::Transform& DebugInfoComponent::getTransform()
    {
        return _transform;
    }

    const DebugInfoComponent::Transform& DebugInfoComponent::
        getTransform() const
    {
        return _transform;
    }

    void DebugInfoComponent::onAddedToEntity(Entity& entity)
    {
        _entity = entity.getSharedPtr();
        _label = entity.getContext().getLabels().load(_font);
    }

    bool DebugInfoComponent::update(double dt)
    {
        if(auto ptr = _entity.lock())
        {
            auto& info = ptr->getContext().getGorn().getQueue().getDebugInfo();
            std::stringstream ss;
            ss << "fps: " << info.framesPerSecond << std::endl;
            ss << "draws: " << info.drawCalls << "/";
            ss << info.drawCallsBatched;
            _label.setText(ss.str());
        }
        _label.update(dt);
        _transform.update();
        return true;
    }

    void DebugInfoComponent::render(gorn::RenderQueue& queue)
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
