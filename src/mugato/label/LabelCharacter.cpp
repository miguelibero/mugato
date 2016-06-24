

#include <mugato/label/LabelCharacter.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <glm/gtx/transform.hpp>

namespace mugato {

    LabelCharacter::LabelCharacter():
    _dirty(true)
    {
    }

    LabelCharacter::LabelCharacter(const std::shared_ptr<gorn::Material>& material):
    _frame(material), _dirty(true)
    {
    }

    LabelCharacter::LabelCharacter(
        const std::shared_ptr<gorn::Material>& material, const Region& region):
        _frame(material, region), _region(region), _dirty(true)
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

    const LabelCharacter::Region& LabelCharacter::getRegion() const
    {
        return _region;
    }

    const std::shared_ptr<gorn::Material>& LabelCharacter::getMaterial() const
    {
        return _frame.getMaterial();
    }

    std::shared_ptr<gorn::Material>& LabelCharacter::getMaterial()
    {
        return _frame.getMaterial();
    }

    void LabelCharacter::setMaterial(const std::shared_ptr<gorn::Material>& material)
    {
        _frame.setMaterial(material);
    }

    void LabelCharacter::update()
    {
        if(_dirty)
        {
            glm::vec3 pos(_region.getAdvance(), 0.0f, 0.0f);
            _transform = glm::translate(glm::mat4(), pos);
            _dirty = false;
        }
        _frame.update();
    }

    void LabelCharacter::render(gorn::RenderQueue& queue) const
    {
        queue.addCommand(_frame.render());

        queue.addCommand()
          .withTransformAction(gorn::RenderTransformStackAction::PushLocal)
          .withTransform(_transform);
    }

}

