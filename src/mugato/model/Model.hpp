

#ifndef __mugato__Model__
#define __mugato__Model__

#include <mugato/model/ModelData.hpp>
#include <gorn/gl/Material.hpp>

namespace gorn {
	class MaterialManager;
	class MaterialDefinition;
	class RenderQueue;
}

namespace mugato {

    class Model
    {
	public:
		typedef ModelData Data;
		typedef Data::MaterialMap MaterialMap;
    private:
		std::shared_ptr<Data> _data;
		MaterialMap _materials;
    public:
		Model();
		Model(const std::shared_ptr<Data>& data, const MaterialMap& materials);

		void setData(const std::shared_ptr<Data>& data);
		std::shared_ptr<Data> getData() const;

		void setMaterials(const MaterialMap& materials);

		void update(double dt);
		void render(gorn::RenderQueue& queue) const;
    };

}

#endif
