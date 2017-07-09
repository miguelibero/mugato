

#include <mugato/model/ModelManager.hpp>
#include <mugato/model/Model.hpp>
#include <mugato/model/ModelDefinition.hpp>
#include <gorn/asset/FileManager.hpp>
#include <gorn/gl/MaterialManager.hpp>

namespace mugato {

    ModelManager::ModelManager(gorn::MaterialManager& materials, gorn::FileManager& files):
    _materials(materials), _datas(files)
    {
		getDefinitions().set([](const std::string& name) {
			return Definition()
				.withData(name);
		});
    }

    const ModelManager::Definitions& ModelManager::getDefinitions() const
    {
        return _definitions;
    }

    ModelManager::Definitions& ModelManager::getDefinitions()
    {
        return _definitions;
    }

    const ModelManager::Datas& ModelManager::getDatas() const
    {
        return _datas;
    }

    ModelManager::Datas& ModelManager::getDatas()
    {
        return _datas;
    }

    Model ModelManager::load(const std::string& dname)
    {
        Model model;
        auto& def = getDefinitions().get(dname);
		auto data = _datas.load(def.getData()).get();
		model.setData(data);

		std::vector<std::string> matNames;
		data->getMaterials(matNames);
		ModelData::MaterialMap modelMaterials;
		for (auto& matName : matNames)
		{
			auto& matDef = _materials.getDefinitions().get(matName);
			def.filterMaterial(matDef);
			modelMaterials[matName] = _materials.load(matName);
		}
		model.setMaterials(modelMaterials);
        return model;
    }

    Model ModelManager::tryLoad(const std::string& name)
    {
        if(!getDefinitions().has(name))
        {
            return Model();
        }
        return load(name);
    }

}
