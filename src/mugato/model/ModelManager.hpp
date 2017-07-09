

#ifndef __mugato__ModelManager__
#define __mugato__ModelManager__

#include <gorn/asset/AssetManager.hpp>
#include <gorn/base/DefinitionManager.hpp>
#include <mugato/model/ModelDefinition.hpp>

namespace gorn {
    class FileManager;
    class MaterialManager;
}

namespace mugato {

    class Model;
	class ModelData;

    class ModelManager
    {
    public:
        typedef ModelData Data;
        typedef ModelDefinition Definition;
        typedef gorn::AssetManager<Data> Datas;
        typedef ModelDefinition Definition;
        typedef gorn::DefinitionManager<Definition> Definitions;
    private:
        gorn::MaterialManager& _materials;
        Datas _datas;
        Definitions _definitions;
    public:
        ModelManager(gorn::MaterialManager& materials, gorn::FileManager& files);
        ModelManager(const ModelManager& other) = delete;

        const Datas& getDatas() const;
        Datas& getDatas();

        const Definitions& getDefinitions() const;
        Definitions& getDefinitions();

        Model load(const std::string& name);
        Model tryLoad(const std::string& name);
    };
}

#endif
