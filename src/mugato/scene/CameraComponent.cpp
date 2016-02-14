#include <mugato/scene/CameraComponent.hpp>
#include <gorn/render/RenderQueue.hpp>

namespace mugato
{
    CameraComponent::CameraComponent()
    {
    }

	CameraComponent::CameraComponent(const glm::mat4& view, const glm::mat4& proj):
	_view(view), _proj(proj)
	{
		updateTransform();
	}

	void CameraComponent::setViewTransform(const glm::mat4& view)
	{
		if (_view != view)
		{
			_view = view;
			updateTransform();
		}
	}

	void CameraComponent::setProjectionTransform(const glm::mat4& proj)
	{
		if (_proj != proj)
		{
			_proj = proj;
			updateTransform();
		}
	}

	void  CameraComponent::updateTransform()
	{
		_transform = _proj*_view;
	}

	void CameraComponent::beforeEntityChildrenRender(gorn::RenderQueue& queue)
	{
		queue.addCommand()
			.withTransformMode(gorn::RenderCommand::TransformMode::PushCheckpoint);
		queue.addCommand()
			.withTransform(_transform)
			.withTransformMode(gorn::RenderCommand::TransformMode::SetGlobal);
	}
	void CameraComponent::afterEntityChildrenRender(gorn::RenderQueue& queue)
	{
		queue.addCommand()
			.withTransformMode(gorn::RenderCommand::TransformMode::PopCheckpoint);
	}

}
