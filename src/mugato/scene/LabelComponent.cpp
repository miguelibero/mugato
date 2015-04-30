
#include <mugato/scene/LabelComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/scene/EntityTransform.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/sprite/SpriteManager.hpp>

namespace mugato
{
    LabelComponent::LabelComponent(const std::string& text,
    const std::string& font):
    _label(text), _font(font), _pivotPercent(0.0f)
    {
    }

    Label& LabelComponent::getLabel()
    {
        return _label;
    }

    const Label& LabelComponent::getLabel() const
    {
        return _label;
    }

    void LabelComponent::setText(const std::string& text)
    {
        _label.setText(text);
    }

    void LabelComponent::setEntityPivotPercent(const glm::vec2& val)
    {
        _pivotPercent = val;
        if(auto ptr = _entity.lock())
        {
            ptr->getTransform().setPivot(_pivotPercent*_label.getSize());
        }
    }

    void LabelComponent::setEntitySize()
    {
        if(auto ptr = _entity.lock())
        {
            ptr->getTransform().setSize(_label.getSize());
        }
    }

    void LabelComponent::onAddedToEntity(Entity& entity)
    {
        _label.setFont(entity.getContext().getLabels().loadFont(_font));
        _entity = entity.getSharedPtr();
        setEntityPivotPercent(_pivotPercent);
        setEntitySize();
    }

    void LabelComponent::update(double dt)
    {
        _label.update(dt);
    }

    void LabelComponent::render(gorn::RenderQueue& queue)
    {
        _label.render(queue);
    }

}
