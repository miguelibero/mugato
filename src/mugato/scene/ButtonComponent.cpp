
#include <mugato/scene/ButtonComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/scene/EntityTransform.hpp>
#include <mugato/base/Context.hpp>
#include <mugato/sprite/SpriteManager.hpp>
#include <gorn/base/Ray.hpp>

namespace mugato
{
    ButtonComponent::ButtonComponent():
	_state(State::Normal)
    {
        _bg.setResizeMode(SpriteResizeMode::Exact);
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

    void ButtonComponent::setTouchCallback(const TouchCallback& cb)
    {
        _touchCallback = cb;
    }

    void ButtonComponent::setClickCallback(const ClickCallback& cb)
    {
        _clickCallback = cb;
    }

    void ButtonComponent::setLabelMaterial(
        const std::shared_ptr<gorn::Material>& material, State state)
    {
        _labelMaterials[state] = material;
    }

    void ButtonComponent::setBackgroundMaterial(
        const std::shared_ptr<gorn::Material>& material, State state)
    {
        _bgMaterials[state] = material;
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

    void ButtonComponent::loadBackground(Context& ctx)
    {
        auto resize = _bg.getResizeMode();
        _bg = ctx.getSprites().tryLoad(_bgName);
        _bg.setResizeMode(resize);
        auto itr = _bgMaterials.find(State::Normal);
        if(itr == _bgMaterials.end())
        {
            _bgMaterials.insert(itr,
                {State::Normal, _bg.getMaterial()});
        }
    }
    void ButtonComponent::loadLabel(Context& ctx)
    {
        auto text = _label.getText();
        auto align = _label.getAlignment();
        _label = ctx.getLabels().tryLoad(_labelName);
        _label.setText(text);
        _label.setAlignment(align);
        auto itr = _labelMaterials.find(State::Normal);
        if(itr == _labelMaterials.end())
        {
            _labelMaterials.insert(itr,
                {State::Normal, _label.getMaterial()});
        }
    }

    void ButtonComponent::onAddedToEntity(Entity& entity)
    {
        auto& ctx = entity.getContext();
        loadBackground(ctx);
        loadLabel(ctx);
        onEntityTransformChanged(entity);
    }

    void ButtonComponent::onEntityTransformChanged(Entity& entity)
    {
        glm::vec2 size(entity.getTransform().getSize());
        _bg.setSize(size);
        _label.setSize(size);
    }

    void ButtonComponent::setState(State state)
    {
		_state = state;
        {
            auto itr = _bgMaterials.find(state);
            if(itr != _bgMaterials.end())
            {
                _bg.setMaterial(itr->second);
            }
        }
        {
            auto itr = _labelMaterials.find(state);
            if(itr != _labelMaterials.end())
            {
                _label.setMaterial(itr->second);
            }
        }
    }

	bool ButtonComponent::onScreenTouched(Entity& entity,
		const gorn::RenderCamera& cam,
		const glm::vec2& p, TouchPhase phase)
    {
		glm::vec3 hp;
		auto r = cam.getScreenPointRay(p);
		r = r.transform(entity.getModelInverse());
		bool hit = r.hits(entity.getTransform().getLocalArea(), hp);
		if(!hit)
		{
			if(_state == State::Pressed)
			{
				phase = TouchPhase::Cancel;
			}
			else
			{
				phase = TouchPhase::None;
			}
		}
        if(phase == TouchPhase::Begin || phase == TouchPhase::Move)
        {
            setState(State::Pressed);
        }
        else
        {
            setState(State::Normal);
        }
        if(phase != TouchPhase::None && _touchCallback)
        {
            hit = _touchCallback(phase, hp);
        }
        if(phase == TouchPhase::End && _clickCallback)
        {
            _clickCallback();
        }
        return hit;
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
