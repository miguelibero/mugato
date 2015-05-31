#ifndef __mugato__LabelComponent__
#define __mugato__LabelComponent__

#include <mugato/scene/Component.hpp>
#include <mugato/label/Label.hpp>
#include <string>
#include <glm/glm.hpp>

namespace mugato
{
    class LabelComponent : public Component
    {
    private:
        Label _label;
        static std::string _defaultName;
        std::string _name;
        glm::mat4 _transform;
    public:
        LabelComponent(const std::string& text);
        LabelComponent(const std::string& text, const std::string& name);

        static void setDefaultName(const std::string& name);

        Label& getLabel();
        const Label& getLabel() const;

        void setText(const std::string& text);

        void onAddedToEntity(Entity& entity) override;
        void onEntityTransformChanged(Entity& entity) override;
        void update(double dt) override;
        void render(gorn::RenderQueue& queue) override;
    };
}

#endif
