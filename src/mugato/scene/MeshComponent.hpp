#ifndef __mugato__MeshComponent__
#define __mugato__MeshComponent__

#include <mugato/scene/Component.hpp>
#include <gorn/asset/Mesh.hpp>

namespace mugato
{
    class MeshComponent : public Component
    {
    private:
		std::shared_ptr<gorn::Mesh> _mesh;
		std::shared_ptr<gorn::Material> _material;
		std::shared_ptr<gorn::Material> _normalMaterial;
		std::string _materialName;
    public:
		MeshComponent(const std::shared_ptr<gorn::Mesh>& mesh, const std::string& material);
		MeshComponent(const std::shared_ptr<gorn::Mesh>& mesh, const std::shared_ptr<gorn::Material>& material);

		void setNormalMaterial(const std::shared_ptr<gorn::Material>& material);
		void onAssignedToContext(Context& ctx) override;
        void onAddedToEntity(Entity& entity) override;
        void onEntityTransformChanged(Entity& entity) override;
        void update(double dt) override;
        void render(gorn::RenderQueue& queue) override;
    };
}

#endif
