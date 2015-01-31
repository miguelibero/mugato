

#include <mugato/label/LabelCharacter.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <glm/gtx/transform.hpp>

namespace mugato {

    LabelCharacter::LabelCharacter()
    {
        init();
    }

    LabelCharacter::LabelCharacter(const std::shared_ptr<gorn::Material>& material):
    SpriteFrame(material)
    {
        init();
    }

    LabelCharacter::LabelCharacter(
        const std::shared_ptr<gorn::Material>& material, const Region& region):
        SpriteFrame(material, region), _region(region)
    {
        init();
    }

    void LabelCharacter::init()
    {
        _dirty = true;
    }

    void LabelCharacter::setRegion(const Region& region)
    {
        if(_region != region)
        {
            _region = region;
            SpriteFrame::setRegion(region);
            _dirty = true;
        }
    }

    const LabelCharacter::Region& LabelCharacter::getRegion() const
    {
        return _region;
    }

    void LabelCharacter::update()
    {
        if(_dirty)
        {
            float x = _region.getAdvance();
            glm::vec3 pos(x, 0.0f, 0.0f);
            _transform = glm::translate(glm::mat4(), pos);
            _dirty = false;
        }
        SpriteFrame::update();
    }

    void LabelCharacter::render(gorn::RenderQueue& queue) const
    {
        SpriteFrame::render(queue);

        queue.addCommand()
          .withTransformMode(gorn::RenderCommand::TransformMode::PushLocal)
          .withTransform(_transform);
    }

}

