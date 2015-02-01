
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <random>
#include <iostream>

namespace gorn
{
	mugato::Context _ctx;
    mugato::QuadTree<void> _quadtree(
        mugato::Rectangle(-1.0, -1.0, 2.0, 2.0), 2);

    std::mt19937 _randomAlgo;
    std::uniform_real_distribution<float> _randomPosDistri(-1.0, 1.0);
    std::uniform_real_distribution<float> _randomSizeDistri(0.0, 0.1);

    VertexArray _vaoElms;
    std::shared_ptr<VertexBuffer> _vboElms;

    VertexArray _vaoNodes;
    std::shared_ptr<VertexBuffer> _vboNodes;

    double _deltaTime = 0.0f;
    size_t _elmsNum = 0;
    size_t _nodesNum = 0;

	Application::Application()
	{
	}

	void Application::load()
	{
#ifdef GORN_PLATFORM_LINUX
		_ctx.getGorn().getFiles()
            .addDefaultLoader<LocalFileLoader>("../assets/%s");
#elif GORN_PLATFORM_ANDROID
		_ctx.getGorn().getFiles()
            .addDefaultLoader<BundleFileLoader>("%s");
#endif

        std::random_device rd;
        _randomAlgo = decltype(_randomAlgo)(rd());

        _vboElms = std::make_shared<VertexBuffer>();
        _vaoElms.setProgram(_ctx.getGorn().getPrograms().load("shader"));
        _vaoElms.setAttribute(_vboElms, AttributeDefinition("position")
            .withType(gorn::BasicType::Float)
            .withCount(2)
            .withStride(2*getSize(gorn::BasicType::Float)));

        _vboNodes = std::make_shared<VertexBuffer>();
        _vaoNodes.setProgram(_ctx.getGorn().getPrograms().load("shader"));
        _vaoNodes.setAttribute(_vboNodes, AttributeDefinition("position")
            .withType(gorn::BasicType::Float)
            .withCount(2)
            .withStride(2*getSize(gorn::BasicType::Float)));
	}

	void Application::unload()
	{
	}

    float randomPos()
    {
        return _randomPosDistri(_randomAlgo);
    }

    float randomSize()
    {
        return _randomSizeDistri(_randomAlgo);
    }

	void Application::update(double dt)
	{
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if(_deltaTime == 0.0f)
        {
            _quadtree.clear();
            for(size_t i=0; i<100; i++)
            {            
                mugato::Rectangle rect(
                    randomPos(),
                    randomPos(),
                    randomSize(),
                    randomSize()
                );
                _quadtree.insert(rect);
            }

            _vboElms->setData(
                _quadtree.getElementsVertices(
                    gorn::DrawMode::Lines),
                VertexBufferUsage::DynamicDraw);

            _vboNodes->setData(
                _quadtree.getNodesVertices(
                    gorn::DrawMode::Lines),
                VertexBufferUsage::DynamicDraw);

            _elmsNum = _quadtree.size();
            _nodesNum = _quadtree.sizeNodes();

            std::cout << _elmsNum << " elements" << std::endl;
            std::cout << _nodesNum << " nodes" << std::endl;

            _deltaTime = 0.0f;
        }
        _deltaTime += dt;
        if(_deltaTime > 1.0f)
        {
            _deltaTime = 0.0f;
        }
        _vaoElms.setUniformValue("color", glm::vec3(1.0f, 0.0f, 0.0f));
        _vaoElms.draw(_elmsNum*8, gorn::DrawMode::Lines);
        _vaoNodes.setUniformValue("color", glm::vec3(0.0f, 1.0f, 0.0f));
        _vaoNodes.draw(_nodesNum*8, gorn::DrawMode::Lines);
	}

}
