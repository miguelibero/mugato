
#include <mugato/scene/LabelComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/scene/EntityTransform.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/sprite/SpriteManager.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace mugato
{
    LabelComponent::LabelComponent(const std::string& text,
    const std::string& font):
    _label(text), _font(font)
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

    void LabelComponent::onAddedToEntity(Entity& entity)
    {
        _label.setFont(entity.getContext().getLabels().loadFont(_font));
        _entity = entity.getSharedPtr();
        onEntityTransformChanged(entity);
    }

    void LabelComponent::onEntityTransformChanged(Entity& entity)
    {
        _label.setSize(glm::vec2(entity.getTransform().getSize()));
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
