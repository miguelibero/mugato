
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <glm/gtc/constants.hpp>
#include <functional>
#include <random>

class SceneApplication : public mugato::Application
{
    typedef std::uniform_real_distribution<float> RandomDistri;
    typedef std::mt19937 RandomAlgo;
    RandomAlgo _randomAlgo;
    RandomDistri _posXDistri;
    RandomDistri _posYDistri;
    RandomDistri _rotDistri;

    float randomPos();

    void moveGuy(std::shared_ptr<mugato::Entity> guy, float duration);

public:

    SceneApplication();

    void load() override;

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
    mugato::Application::load();

#ifdef GORN_PLATFORM_LINUX
	getGorn().getFiles()
        .makeDefaultLoader<gorn::LocalFileLoader>("../assets/%s");
#elif GORN_PLATFORM_ANDROID
	getGorn().getFiles()
        .makeDefaultLoader<gorn::AssetFileLoader>("%s");
#elif GORN_PLATFORM_IOS
    getGorn().getFiles()
    .makeDefaultLoader<gorn::BundleFileLoader>("%s");
#endif

	getGorn().getImages()
        .makeDefaultDataLoader<gorn::StbImageLoader>();

    glm::vec2 screenSize(480.0f, 320.0f);
    getMugato().setScreenSize(screenSize);

    auto& matdefs = getGorn().getMaterials().getDefinitions();
    matdefs.set("octree_elements", gorn::MaterialDefinition()
        .withUniformValue(gorn::UniformKind::Color,
            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))
        .withProgram(mugato::ProgramKind::Color));

    matdefs.set("octree_nodes", gorn::MaterialDefinition()
        .withUniformValue(gorn::UniformKind::Color,
            glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))
        .withProgram(mugato::ProgramKind::Color));

    auto scene = getMugato().getScenes().push();
    auto& bg = scene->addComponent<mugato::SpriteComponent>("background.png");
    auto bgsize = bg.getSprite().getSize();
    scene->getTransform().setScale(screenSize/bgsize);

    scene->addComponent<mugato::RenderInfoComponent>();

    auto& materials = getGorn().getMaterials();
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
        auto& sprite = guy->addComponent<mugato::SpriteComponent>(
            "character.png").getSprite();
        guy->getTransform().setScale(0.25f);
        guy->getTransform().setSize(sprite.getContentSize());
        guy->getTransform().setRotation(_rotDistri(_randomAlgo));
        guy->getTransform().setPosition(glm::vec2(
            _posXDistri(_randomAlgo),
            _posYDistri(_randomAlgo)));
        moveGuy(guy, 4.0f);
    }
}

void SceneApplication::moveGuy(
    std::shared_ptr<mugato::Entity> guy, float duration)
{
    auto& actions = guy->addComponent<mugato::ActionComponent>();
    auto endt = guy->getTransform();
    endt.setPosition(glm::vec2(
        _posXDistri(_randomAlgo),
        _posYDistri(_randomAlgo)));
    endt.setRotation(_rotDistri(_randomAlgo));
    actions.add<mugato::TweenAction>(duration, endt)
        .withComplete(std::bind(
            &SceneApplication::moveGuy, this, guy, duration));
}

