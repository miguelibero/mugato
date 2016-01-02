#ifndef __mugato__ButtonComponent__
#define __mugato__ButtonComponent__

#include <mugato/scene/Component.hpp>
#include <mugato/sprite/Sprite.hpp>
#include <mugato/label/Label.hpp>
#include <string>

namespace mugato
{
    class ButtonComponent : public Component
    {
    public:
        typedef std::function<bool(Entity& entity, const glm::vec2& p,
            TouchPhase phase)> Callback;
    private:
        Sprite _bg;
        Label _label;
        Callback _callback;
        std::string _bgName;
        std::string _labelName;
    public:
        ButtonComponent();

        void setBackground(const std::string& name);
        void setText(const std::string& text);
        void setLabel(const std::string& name);
        void setCallback(const Callback& cb);

        Sprite& getBackground();
        const Sprite& getBackground() const;
        Label& getLabel();
        const Label& getLabel() const;

        void onAssignedToContext(Context& ctx) override;
        void onAddedToEntity(Entity& entity) override;
        void onEntityTransformChanged(Entity& entity) override;
        bool onEntityTouched(Entity& entity, const glm::vec2& p,
            TouchPhase phase) override;
        void update(double dt) override;
        void render(gorn::RenderQueue& queue) override;
    };
}

#endif
