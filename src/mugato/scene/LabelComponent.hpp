#ifndef __mugato__LabelComponent__
#define __mugato__LabelComponent__

#include <mugato/scene/Component.hpp>
#include <mugato/label/Label.hpp>
#include <string>

namespace mugato
{
    class LabelComponent : public Component
    {
    private:
        Label _label;
        std::string _font;
        std::weak_ptr<Entity> _entity;
        glm::vec2 _pivotPercent;
    public:
        LabelComponent(const std::string& text, const std::string& font="");

        Label& getLabel();
        const Label& getLabel() const;

        void setText(const std::string& text);

        void setEntityPivotPercent(const glm::vec2& val);
        void setEntitySize();

        void onAddedToEntity(Entity& entity) override;
        void update(double dt) override;
        void render(gorn::RenderQueue& queue) override;
    };
}

#endif
