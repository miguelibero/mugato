
#include <mugato/scene/LabelComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/scene/EntityTransform.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/sprite/SpriteManager.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace mugato
{
    std::string LabelComponent::_defaultName = "default";

    LabelComponent::LabelComponent(const std::string& text,
    const std::string& name):
    _label(text), _name(name)
    {
    }

    LabelComponent::LabelComponent(const std::string& text):
    _label(text), _name(_defaultName)
    {
    }

    void LabelComponent::setDefaultName(const std::string& name)
    {
        _defaultName = name;
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
        auto font = entity.getContext().getLabels().loadLabelFont(_name);
        _label.setFont(font);
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
