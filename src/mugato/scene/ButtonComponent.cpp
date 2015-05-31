
#include <mugato/scene/ButtonComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/scene/EntityTransform.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/sprite/SpriteManager.hpp>

namespace mugato
{
    ButtonComponent::ButtonComponent()
    {
    }

    void ButtonComponent::setBackground(const std::string& name)
    {
        _bgName = name;
    }

    void ButtonComponent::setText(const std::string& text)
    {
        _label.setText(text);
    }

    void ButtonComponent::setLabel(const std::string& name)
    {
        _labelName = name;
    }

    void ButtonComponent::setCallback(const Callback& cb)
    {
        _callback = cb;
    }

    Sprite& ButtonComponent::getBackground()
    {
        return _bg;
    }

    const Sprite& ButtonComponent::getBackground() const
    {
        return _bg;
    }

    Label& ButtonComponent::getLabel()
    {
        return _label;
    }

    const Label& ButtonComponent::getLabel() const
    {
        return _label;
    }

    void ButtonComponent::onAddedToEntity(Entity& entity)
    {
        _bg = entity.getContext().getSprites().load(_bgName);
        _bg.setResizeMode(SpriteResizeMode::Exact);
        auto text = _label.getText();
        _label = entity.getContext().getLabels().load(_labelName);
        _label.setText(text);
        _label.setAlignment(LabelAlignment::Center);
        onEntityTransformChanged(entity);
    }

    void ButtonComponent::onEntityTransformChanged(Entity& entity)
    {
        glm::vec2 size(entity.getTransform().getSize());
        _bg.setSize(size);
        _label.setSize(size);
    }

    bool ButtonComponent::onEntityTouched(Entity& entity, const glm::vec2& p,
        TouchPhase phase)
    {
        if(_callback)
        {
            return _callback(entity, p, phase);
        }
        return false;
    }

    void ButtonComponent::update(double dt)
    {
        _bg.update(dt);
        _label.update(dt);
    }

    void ButtonComponent::render(gorn::RenderQueue& queue)
    {
        _bg.render(queue);
        _label.render(queue);
    }

}
