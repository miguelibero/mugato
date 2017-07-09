
#include <mugato/model/ModelDefinition.hpp>

namespace mugato
{
    ModelDefinition::ModelDefinition()
    {
    }

	ModelDefinition& ModelDefinition::withData(const std::string& data)
	{
		_data = data;
		return *this;
	}

	ModelDefinition& ModelDefinition::withMaterialFilter(const MaterialDefinitionFilter& filter)
	{
		_materialFilter = filter;
		return *this;
	}

	const std::string& ModelDefinition::getData() const
	{
		return _data;
	}

	void ModelDefinition::filterMaterial(gorn::MaterialDefinition& matDef)
	{
		if (_materialFilter != nullptr)
		{
			_materialFilter(matDef);
		}
	}
}
