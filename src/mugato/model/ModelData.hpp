

#ifndef __mugato__ModelData__
#define __mugato__ModelData__

#include <gorn/gl/MaterialDefinition.hpp>
#include <gorn/asset/Mesh.hpp>
#include <vector>
#include <map>

namespace gorn {
	class RenderQueue;
}

namespace mugato {

	struct ModelDataMesh
	{
		std::shared_ptr<gorn::Mesh> mesh;
		std::string material;
	};

    class ModelData
    {
	public:
		typedef ModelDataMesh Mesh;
		typedef std::vector<Mesh> Meshes;
		typedef std::vector<std::unique_ptr<ModelData>> Children;
		typedef std::map<std::string, std::shared_ptr<gorn::Material>> MaterialMap;
    private:
		glm::mat4 _transform;
		Meshes _meshes;
		Children _children;
    public:
		ModelData();

		void setTransform(const glm::mat4& mat);
        void addMesh(const Mesh& mesh);
        void addChild(ModelData&& child);

		const glm::mat4& getTransform() const;
		const Meshes& getMeshes() const;
		Meshes& getMeshes();
		const Children& getChildren() const;
		Children& getChildren();

		void getMaterials(std::vector<std::string>& materials) const;

		void render(gorn::RenderQueue& queue, const MaterialMap& materials) const;
    };

}

#endif
