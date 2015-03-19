
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <glm/gtc/constants.hpp>

class SceneApplication : public gorn::Application
{
    typedef std::uniform_real_distribution<float> RandomDistri;
    typedef std::mt19937 RandomAlgo;
	mugato::Context _ctx;
    RandomAlgo _randomAlgo;
    RandomDistri _posXDistri;
    RandomDistri _posYDistri;
    RandomDistri _rotDistri;

    float randomPos();

public:

    SceneApplication();

    void load() override;
    void update(double dt) override;
    void draw() override;

};

namespace gorn
{
    std::unique_ptr<Application> main()
    {
        return std::unique_ptr<Application>(new SceneApplication());
    }
}

SceneApplication::SceneApplication()
{
}

void SceneApplication::load()
{
#ifdef GORN_PLATFORM_LINUX
	_ctx.getGorn().getFiles()
        .addDefaultLoader<gorn::LocalFileLoader>("../assets/%s");
	_ctx.getGorn().getImages()
        .addDefaultDataLoader<gorn::PngImageLoader>();
#elif GORN_PLATFORM_ANDROID
	_ctx.getGorn().getFiles()
        .addDefaultLoader<gorn::BundleFileLoader>("%s");
	_ctx.getGorn().getImages()
        .addDefaultDataLoader<gorn::GraphicsImageLoader>();
#endif

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _ctx.setScreenSize(glm::vec2(480.0f, 320.0f));

    auto& matdefs = _ctx.getGorn().getMaterials().getDefinitions();
    matdefs.set("octree_elements", gorn::MaterialDefinition()
        .withUniformValue(gorn::UniformKind::Color,
            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))
        .withProgram(mugato::ProgramKind::Color));
    matdefs.set("octree_nodes", gorn::MaterialDefinition()
        .withUniformValue(gorn::UniformKind::Color,
            glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))
        .withProgram(mugato::ProgramKind::Color));

    auto scene = _ctx.getScenes().push();
    auto& bg = scene->addComponent<mugato::SpriteComponent>("background.png");
    bg.setEntityPivotPercent(glm::vec2(0.0f));
    auto bgsize = bg.getSprite().getSize();
    scene->getTransform().setScale(_ctx.getScreenSize()/bgsize);
    
    auto& materials = _ctx.getGorn().getMaterials();
    auto& octree = scene->addComponent<mugato::OcTreeRenderComponent>();
    octree.setElementsMaterial(materials.load("octree_elements"));
    octree.setElementsDrawMode(gorn::DrawMode::Lines);
    octree.setNodesMaterial(materials.load("octree_nodes"));
    octree.setNodesDrawMode(gorn::DrawMode::Lines);

    _randomAlgo = RandomAlgo(time(0));
    _posXDistri = RandomDistri(0.0f, bgsize.x);
    _posYDistri = RandomDistri(0.0f, bgsize.y);
    _rotDistri = RandomDistri(0.0f, glm::pi<float>()*2.0f);

    for(int i=0; i<100; ++i)
    {
        auto guy = scene->addChild();
        guy->addComponent<mugato::SpriteComponent>("character.png");
        guy->getTransform().setScale(0.5f);
        guy->getTransform().setRotation(_rotDistri(_randomAlgo));
        guy->getTransform().setPosition(glm::vec2(
            _posXDistri(_randomAlgo),
            _posYDistri(_randomAlgo)));

        auto& actions = guy->addComponent<mugato::ActionComponent>();
        auto endt = guy->getTransform();
        endt.setPosition(glm::vec2(
            _posXDistri(_randomAlgo),
            _posYDistri(_randomAlgo)));
        endt.setRotation(_rotDistri(_randomAlgo));
        actions.add<mugato::TweenAction>(10.0f, endt);
    }
}

void SceneApplication::update(double dt)
{
    _ctx.update(dt);
}

void SceneApplication::draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
   _ctx.draw();
}


