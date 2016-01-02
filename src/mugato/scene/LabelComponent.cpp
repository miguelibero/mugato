
#include <mugato/scene/LabelComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/scene/EntityTransform.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/sprite/SpriteManager.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace mugato
{
    LabelComponent::LabelComponent(const std::string& text,
    const std::string& name):
    _label(text), _name(name)
    {
    }

    LabelComponent::LabelComponent(const std::string& text):
    _label(text)
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

    void LabelComponent::onAssignedToContext(Context& ctx)
    {
        auto font = ctx.getLabels().loadLabelFont(_name);
        _label.setFont(font);
    }

    void LabelComponent::onAddedToEntity(Entity& entity)
    {
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
