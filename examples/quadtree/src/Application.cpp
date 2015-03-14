
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <random>
#include <iostream>

class QuadTreeApplication : public gorn::Application
{
	mugato::Context _ctx;
    mugato::OcTree<void> _quadtree;

    std::mt19937 _randomAlgo;
    std::uniform_real_distribution<float> _randomPosDistri;
    std::uniform_real_distribution<float> _randomSizeDistri;

    gorn::VertexArray _vaoElms;
    std::shared_ptr<gorn::VertexBuffer> _vboElms;

    gorn::VertexArray _vaoNodes;
    std::shared_ptr<gorn::VertexBuffer> _vboNodes;

    double _deltaTime = 0.0f;

    float randomPos();
    float randomSize();

public:

    QuadTreeApplication();

    void load() override;
    void update(double dt) override;
    void draw() override;

};

namespace gorn
{
    std::unique_ptr<Application> main()
    {
        return std::unique_ptr<Application>(new QuadTreeApplication());
    }
}

QuadTreeApplication::QuadTreeApplication():
_quadtree(mugato::Rectangle(glm::vec2(-1.0), glm::vec2(2.0)), 2),
_randomPosDistri(-1.0, 1.0),
_randomSizeDistri(0.0, 0.1)
{
}

void QuadTreeApplication::load()
{
#ifdef GORN_PLATFORM_LINUX
	_ctx.getGorn().getFiles()
        .addDefaultLoader<gorn::LocalFileLoader>("../assets/%s");
#elif GORN_PLATFORM_ANDROID
	_ctx.getGorn().getFiles()
        .addDefaultLoader<gorn::BundleFileLoader>("%s");
#endif

    std::random_device rd;
    _randomAlgo = decltype(_randomAlgo)(rd());

    _vboElms = std::make_shared<gorn::VertexBuffer>();
    _vaoElms.setProgram(_ctx.getGorn().getPrograms().load("shader"));
    _vaoElms.setAttribute(_vboElms, gorn::AttributeDefinition("position")
        .withType(gorn::BasicType::Float)
        .withCount(2)
        .withStride(2, gorn::BasicType::Float));

    _vboNodes = std::make_shared<gorn::VertexBuffer>();
    _vaoNodes.setProgram(_ctx.getGorn().getPrograms().load("shader"));
    _vaoNodes.setAttribute(_vboNodes, gorn::AttributeDefinition("position")
        .withType(gorn::BasicType::Float)
        .withCount(2)
        .withStride(2, gorn::BasicType::Float));
}


float QuadTreeApplication::randomPos()
{
    return _randomPosDistri(_randomAlgo);
}

float QuadTreeApplication::randomSize()
{
    return _randomSizeDistri(_randomAlgo);
}

void QuadTreeApplication::update(double dt)
{
    if(_deltaTime == 0.0f)
    {
        _quadtree.clear();
        for(size_t i=0; i<1000; i++)
        {            
            mugato::Rectangle rect(
                glm::vec2(randomPos(), randomPos()),
                glm::vec2(randomSize(), randomSize())
            );
            _quadtree.insert(rect);
        }

        _vboElms->setData(
            _quadtree.getElementsVertices(gorn::DrawMode::Lines),
            gorn::VertexBufferUsage::DynamicDraw);

        _vboNodes->setData(
            _quadtree.getNodesVertices(gorn::DrawMode::Lines),
            gorn::VertexBufferUsage::DynamicDraw);

        std::cout << _quadtree.size() << " elements" << std::endl;
        std::cout << _quadtree.sizeNodes() << " nodes" << std::endl;

        _deltaTime = 0.0f;
    }
    _deltaTime += dt;
    if(_deltaTime > 1.0f)
    {
        _deltaTime = 0.0f;
    }

}

void QuadTreeApplication::draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    _vaoElms.setUniformValue("color", glm::vec3(1.0f, 0.0f, 0.0f));
    _vaoElms.draw(_quadtree.size()*8, gorn::DrawMode::Lines);
    _vaoNodes.setUniformValue("color", glm::vec3(0.0f, 1.0f, 0.0f));
    _vaoNodes.draw(_quadtree.sizeNodes()*8, gorn::DrawMode::Lines);
}

