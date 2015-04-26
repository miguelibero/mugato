
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

    gorn::Mesh _elmsMesh;
    gorn::Mesh _nodesMesh;

    double _deltaTime;

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
_quadtree(gorn::Rect(glm::vec2(-1.0f), glm::vec2(2.0f)), 2),
_randomPosDistri(-1.0f, 1.0f),
_randomSizeDistri(0.0f, 0.1f),
_deltaTime(0.0f)
{
}

void QuadTreeApplication::load()
{
#ifdef GORN_PLATFORM_LINUX
	_ctx.getGorn().getFiles()
        .makeDefaultLoader<gorn::LocalFileLoader>("../assets/%s");
#elif GORN_PLATFORM_ANDROID
	_ctx.getGorn().getFiles()
        .makeDefaultLoader<gorn::AssetFileLoader>("%s");
#endif

    std::random_device rd;
    _randomAlgo = decltype(_randomAlgo)(rd());

    auto& matdefs = _ctx.getGorn().getMaterials().getDefinitions();

    matdefs.set("octree_elements", gorn::MaterialDefinition()
        .withUniformValue(gorn::UniformKind::Color,
            glm::vec3(1.0f, 0.0f, 0.0f))
        .withProgram("shader"));

    matdefs.set("octree_nodes", gorn::MaterialDefinition()
        .withUniformValue(gorn::UniformKind::Color,
            glm::vec3(0.0f, 0.0f, 1.0f))
        .withProgram("shader"));
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
        for(size_t i=0; i<100; i++)
        {            
            gorn::Rect rect(
                glm::vec2(randomPos(), randomPos()),
                glm::vec2(randomSize(), randomSize())
            );
            _quadtree.insert(rect);
        }
        _quadtree.adjust();

        _elmsMesh = gorn::ShapeMeshFactory::create(
            _quadtree.getElementsRects(), gorn::DrawMode::Lines);
        
        _nodesMesh = gorn::ShapeMeshFactory::create(
            _quadtree.getNodesRects(), gorn::DrawMode::Lines);

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

    auto& queue = _ctx.getGorn().getQueue();
    auto& mats = _ctx.getGorn().getMaterials();

    queue.addCommand(_elmsMesh.render())
        .withMaterial(mats.load("octree_elements"));
    queue.addCommand(_nodesMesh.render())
        .withMaterial(mats.load("octree_nodes"));

    queue.draw();
}

