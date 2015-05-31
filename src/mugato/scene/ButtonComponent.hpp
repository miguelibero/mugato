#ifndef __mugato__ButtonComponent__
#define __mugato__ButtonComponent__

#include <mugato/scene/Component.hpp>
#include <mugato/sprite/Sprite.hpp>
#include <mugato/label/Label.hpp>
#include <string>

namespace mugato
{
    enum class ButtonState
    {
        Pressed,
        Normal  
    };

    class ButtonComponent : public Component
    {
    public:
        typedef ButtonState State;
        typedef std::function<bool(TouchPhase phase,
            const glm::vec2& p)> TouchCallback;
        typedef std::function<void()> ClickCallback;
    private:
        Sprite _bg;
        Label _label;
        TouchCallback _touchCallback;
        ClickCallback _clickCallback;
        std::string _bgName;
        std::string _labelName;
        std::map<State, std::shared_ptr<gorn::Material>> _labelMaterials;
        std::map<State, std::shared_ptr<gorn::Material>> _bgMaterials;

        void setState(State state);
    public:
        ButtonComponent();

        void setBackground(const std::string& name);
        void setText(const std::string& text);
        void setLabel(const std::string& name);
        void setTouchCallback(const TouchCallback& cb);
        void setClickCallback(const ClickCallback& cb);
        void setLabelMaterial(
            const std::shared_ptr<gorn::Material>& material, State state);
        void setBackgroundMaterial(
            const std::shared_ptr<gorn::Material>& material, State state);

        Sprite& getBackground();
        const Sprite& getBackground() const;
        Label& getLabel();
        const Label& getLabel() const;

        void onAddedToEntity(Entity& entity) override;
        void onEntityTransformChanged(Entity& entity) override;
        bool onEntityTouched(Entity& entity, const glm::vec2& p,
            TouchPhase phase) override;
        void update(double dt) override;
        void render(gorn::RenderQueue& queue) override;
    };
}

#endif
