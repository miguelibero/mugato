
#include <mugato/scene/MeshComponent.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/scene/EntityTransform.hpp>
#include <mugato/base/Context.hpp>

namespace mugato
{
    MeshComponent::MeshComponent(const std::shared_ptr<gorn::Mesh>& mesh, const std::string& material):
	_mesh(mesh), _materialName(material)
    {
    }

	MeshComponent::MeshComponent(const std::shared_ptr<gorn::Mesh>& mesh, const std::shared_ptr<gorn::Material>& material):
	_mesh(mesh), _material(material)
	{
	}

	void MeshComponent::setNormalMaterial(const std::shared_ptr<gorn::Material>& material)
	{
		_normalMaterial = material;
	}

	void MeshComponent::onAssignedToContext(Context& ctx)
	{
		if(!_materialName.empty())
		{
			_material = ctx.getGorn().getMaterials().load(_materialName);
		}
	}

    void MeshComponent::onAddedToEntity(Entity& entity)
    {
    }

    void MeshComponent::onEntityTransformChanged(Entity& entity)
    {
    }
   
    void MeshComponent::update(double dt)
    {
    }

    void MeshComponent::render(gorn::RenderQueue& queue)
    {
		if(_material)
		{
			auto cmd = _mesh->render();
			cmd.withMaterial(_material);
			queue.addCommand(std::move(cmd));
		}
		if(_normalMaterial)
		{
			auto normals = _mesh->getNormalsMesh();
			auto cmd = normals.render();
			cmd.withMaterial(_normalMaterial);
			queue.addCommand(std::move(cmd));
		}
    }

}
