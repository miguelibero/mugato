#ifndef __mugato__AssimpModelDataLoader__
#define __mugato__AssimpModelDataLoader__

#include <gorn/asset/DataAssetLoader.hpp>
#include <mugato/model/Model.hpp>
#include <memory>
#include <vector>

namespace gorn
{
	class MaterialManager;
    class MaterialDefinition;
	class Mesh;
}

struct aiScene;
struct aiMaterial;
struct aiMesh;
struct aiNode;

namespace mugato
{
	class Entity;

	class AssimpModelDataLoader : public gorn::DataAssetLoader<ModelData>
	{
	private:
		gorn::MaterialManager& _materials;

        gorn::MaterialDefinition loadMaterial(const aiMaterial* aimaterial) const;
		gorn::Mesh loadMesh(const aiMesh* aimesh) const;
		ModelData loadNode(const aiNode* node, const std::vector<ModelDataMesh>& meshes) const;
	public:
        AssimpModelDataLoader(gorn::MaterialManager& materials) NOEXCEPT;
		bool validate(const buffer& data) const NOEXCEPT override;
		ModelData load(const buffer& data) const override;
	};
}

#endif
