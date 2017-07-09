
#include <mugato/model/ModelData.hpp>
#include <gorn/render/RenderQueue.hpp>

namespace mugato
{
	ModelData::ModelData()
	{
	}

	void ModelData::setTransform(const glm::mat4& mat)
	{
		_transform = mat;
	}

	void ModelData::addMesh(const Mesh& mesh)
	{
		_meshes.push_back(mesh);
	}

	void ModelData::addChild(ModelData&& child)
	{
		_children.push_back(std::make_unique<ModelData>(std::move(child)));
	}

	const glm::mat4& ModelData::getTransform() const
	{
		return _transform;
	}

	const ModelData::Meshes& ModelData::getMeshes() const
	{
		return _meshes;
	}

	ModelData::Meshes& ModelData::getMeshes()
	{
		return _meshes;
	}

	const ModelData::Children& ModelData::getChildren() const
	{
		return _children;
	}

	ModelData::Children& ModelData::getChildren()
	{
		return _children;
	}

	void ModelData::getMaterials(std::vector<std::string>& materials) const
	{
		for (auto& mesh : _meshes)
		{
			materials.push_back(mesh.material);
			
		}
		for (auto& child : _children)
		{
			child->getMaterials(materials);
		}
	}

	void ModelData::render(gorn::RenderQueue& queue, const MaterialMap& materials) const
	{
		for (auto& mesh : _meshes)
		{
			auto cmd = mesh.mesh->render();
			cmd.withTransform(_transform);
			auto itr = materials.find(mesh.material);
			if (itr != materials.end())
			{
				cmd.withMaterial(itr->second);
			}
			queue.addCommand(std::move(cmd));
		}		
		for (auto& child : _children)
		{
			child->render(queue, materials);
		}
	}
}
