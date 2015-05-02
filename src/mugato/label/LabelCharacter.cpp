

#include <mugato/label/LabelCharacter.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <glm/gtx/transform.hpp>

namespace mugato {

    LabelCharacter::LabelCharacter():
    _mode(Mode::Character), _dirty(true)
    {
    }

    LabelCharacter::LabelCharacter(const glm::vec2& base, Mode mode):
    _base(base), _mode(mode), _dirty(true)
    {
    }

    LabelCharacter::LabelCharacter(const std::shared_ptr<gorn::Material>& material):
    _frame(material), _mode(Mode::Character), _dirty(true)
    {
    }

    LabelCharacter::LabelCharacter(
        const std::shared_ptr<gorn::Material>& material, const Region& region):
        _frame(material, region), _region(region),
        _mode(Mode::Character), _dirty(true)
    {
    }

    void LabelCharacter::setRegion(const Region& region)
    {
        if(_region != region)
        {
            _region = region;
            _frame.setRegion(region);
            _dirty = true;
        }
    }

    LabelCharacter::Mode LabelCharacter::getMode() const
    {
        return _mode;
    }

    const glm::vec2& LabelCharacter::getBase() const
    {
        return _base;
    }

    const LabelCharacter::Region& LabelCharacter::getRegion() const
    {
        return _region;
    }

    void LabelCharacter::update()
    {
        if(_dirty)
        {
            glm::vec3 pos(_base, 0.0f);
            pos.x += _region.getAdvance();
            _transform = glm::translate(glm::mat4(), pos);
            _dirty = false;
        }
        _frame.update();
    }

    void LabelCharacter::render(gorn::RenderQueue& queue) const
    {
        _frame.render(queue);

        if(_mode == Mode::Line)
        {
            queue.addCommand()
              .withTransformMode(gorn::RenderCommand::TransformMode::PopCheckpoint);
        }

        queue.addCommand()
          .withTransformMode(gorn::RenderCommand::TransformMode::PushLocal)
          .withTransform(_transform);

        if(_mode == Mode::Line)
        {
            queue.addCommand()
              .withTransformMode(gorn::RenderCommand::TransformMode::PushCheckpoint);
        }
    }

}

