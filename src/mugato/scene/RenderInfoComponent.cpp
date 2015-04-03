#include <mugato/scene/RenderInfoComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/base/Context.hpp>
#include <gorn/base/Context.hpp>
#include <sstream>

namespace mugato {

    RenderInfoComponent::RenderInfoComponent(const std::string& font):
    _font(font)
    {
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

    void RenderInfoComponent::onAddedToEntity(Entity& entity)
    {
        _entity = entity.getSharedPtr();
        _label = entity.getContext().getLabels().load(_font);
    }

    void RenderInfoComponent::update(double dt)
    {
        if(auto ptr = _entity.lock())
        {
            auto& info = ptr->getContext().getGorn().getQueue().getInfo();
            std::stringstream ss;
            ss << "fps: " << info.framesPerSecond << std::endl;
            ss << "draws: " << info.drawCalls << "/";
            ss << info.drawCallsBatched << "/";
            ss << info.drawCallsCulled << std::endl;
            ss << "verts: " << info.vertexCount;
            _label.setText(ss.str());
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
