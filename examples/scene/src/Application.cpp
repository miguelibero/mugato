
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <glm/gtc/constants.hpp>

class SceneApplication : public gorn::Application
{
	mugato::Context _ctx;
    std::shared_ptr<mugato::Entity> _guy1;
    std::shared_ptr<mugato::Entity> _guy2;

public:

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
    scene->setScale(_ctx.getScreenSize()/bg.getSprite().getSize());
    
    auto& materials = _ctx.getGorn().getMaterials();
    auto& octree = scene->addComponent<mugato::OcTreeRenderComponent>();
    octree.setElementsMaterial(materials.load("octree_elements"));
    octree.setElementsDrawMode(gorn::DrawMode::Lines);
    octree.setNodesMaterial(materials.load("octree_nodes"));
    octree.setNodesDrawMode(gorn::DrawMode::Lines);

    _guy1 = scene->addChild();
    _guy1->addComponent<mugato::SpriteComponent>("character.png");
    _guy1->setRotation(glm::pi<float>()/4.0f);
    _guy1->setPosition(glm::vec2(4.0f, 160.0f));

    _guy2 = scene->addChild();
    _guy2->addComponent<mugato::SpriteComponent>("character.png");
    _guy2->setRotation(glm::pi<float>()/-4.0f);
    _guy2->setPosition(glm::vec2(4.0f, 160.0f));
}

void SceneApplication::update(double dt)
{
    auto pos = _guy1->getPosition();
    pos.x += 30.0f*dt;
    _guy1->setPosition(pos);

    pos = _guy2->getPosition();
    pos.y += 30.0f*dt;
    _guy2->setPosition(pos);

    _ctx.update(dt);
}

void SceneApplication::draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
   _ctx.draw();
}


