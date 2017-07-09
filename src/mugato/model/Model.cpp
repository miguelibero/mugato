
#include <mugato/model/Model.hpp>
#include <gorn/render/RenderQueue.hpp>
#include <gorn/gl/MaterialManager.hpp>

namespace mugato
{
	Model::Model()
	{
	}

	Model::Model(const std::shared_ptr<Data>& data, const MaterialMap& materials):
	_data(data), _materials(materials)
	{
	}

	void Model::setData(const std::shared_ptr<Data>& data)
	{
		_data = data;
	}

	std::shared_ptr<Model::Data> Model::getData() const
	{
		return _data;
	}

	void Model::setMaterials(const MaterialMap& materials)
	{
		_materials = materials;		
	}

	void Model::update(double dt)
	{
	}

	void Model::render(gorn::RenderQueue& queue) const
	{
		_data->render(queue, _materials);
	}
}
