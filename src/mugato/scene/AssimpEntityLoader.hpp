#ifndef __mugato__AssimpEntityLoader__
#define __mugato__AssimpEntityLoader__

#include <gorn/asset/DataAssetLoader.hpp>
#include <memory>
#include <vector>

namespace gorn
{
	class MaterialManager;
	class Material;
	class Mesh;
}

struct aiScene;
struct aiMaterial;
struct aiMesh;
struct aiNode;

namespace mugato
{
	class Entity;

	struct AssimpContext
	{
		const aiScene* scene;
		std::vector<std::shared_ptr<gorn::Material>> materials;
		std::vector<std::shared_ptr<gorn::Mesh>> meshes;
	};

	class AssimpEntityLoader : public gorn::DataAssetLoader<std::shared_ptr<Entity>>
	{
	private:
		gorn::MaterialManager& _materials;
        std::string _prefix;

		std::shared_ptr<gorn::Material> loadMaterial(const aiMaterial* aimaterial) const;
		std::shared_ptr<gorn::Mesh> loadMesh(const aiMesh* aimesh) const;
		std::shared_ptr<Entity> loadNode(const aiNode* node, const AssimpContext& ctx) const;
	public:
		AssimpEntityLoader(gorn::MaterialManager& materials, const std::string& prefix="assimp") NOEXCEPT;
		bool validate(const buffer& data) const NOEXCEPT override;
		std::shared_ptr<Entity> load(const buffer& data) const override;
	};
}

#endif
