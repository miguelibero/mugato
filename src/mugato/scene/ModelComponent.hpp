#ifndef __mugato__ModelComponent__
#define __mugato__ModelComponent__

#include <mugato/scene/Component.hpp>
#include <mugato/model/Model.hpp>
#include <mugato/sprite/SpriteEnums.hpp>
#include <string>

namespace mugato
{
    class ModelComponent : public Component
    {
    private:
        std::string _name;
        Model _model;
    public:
		ModelComponent(const std::string& name);

        Model& getModel();
        const Model& getModel() const;

        void onAssignedToContext(Context& ctx) override;
        void onAddedToEntity(Entity& entity) override;
        void onEntityTransformChanged(Entity& entity) override;
        void update(double dt) override;
        void render(gorn::RenderQueue& queue) override;
    };
}

#endif
