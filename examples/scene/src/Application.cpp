
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <glm/gtc/constants.hpp>

class SceneApplication : public gorn::Application
{
	mugato::Context _ctx;
    std::shared_ptr<mugato::Entity> _guy;

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

    auto scene = _ctx.getScenes().push();
    auto& bg = scene->addComponent<mugato::SpriteComponent>("background.png");
    scene->setScale(_ctx.getScreenSize()/bg.getSprite().getSize());

    _guy = scene->addChild();
    auto& guySprite = _guy->addComponent<mugato::SpriteComponent>("character.png");
    guySprite.setPivotPercent(glm::vec2(0.5f));
    _guy->setRotation(glm::pi<float>()/2.0f);
    _guy->setPosition(glm::vec2(200.0f, 100.0f));
    _guy->setScale(2.0f);
}

void SceneApplication::update(double dt)
{
    auto pos = _guy->getPosition();
    pos.x += 30.0f*dt;
    _guy->setPosition(pos);

   _ctx.update(dt);
}

void SceneApplication::draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

   _ctx.draw();
}


