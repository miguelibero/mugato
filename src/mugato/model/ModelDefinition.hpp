

#ifndef __mugato__ModelDefinition__
#define __mugato__ModelDefinition__

#include <string>
#include <functional>

namespace gorn {
	class MaterialDefinition;
}

namespace mugato {

    class ModelDefinition
    {
	public:
		typedef std::function<void(gorn::MaterialDefinition&)> MaterialDefinitionFilter;
	private:
		std::string _data;
		MaterialDefinitionFilter _materialFilter;
    public:
        ModelDefinition();

		ModelDefinition& withData(const std::string& data);
		ModelDefinition& withMaterialFilter(const MaterialDefinitionFilter& filter);

		const std::string& getData() const;

		void filterMaterial(gorn::MaterialDefinition& matDef);
    };

}

#endif
