
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

class LabelApplication : public gorn::Application
{
	mugato::Context _ctx;
    mugato::Label _label1;
    mugato::Label _label2;
public:

    void load() override;
    void update(double dt) override;
    void draw() override;
};

namespace gorn
{
    std::unique_ptr<Application> main()
    {
        return std::unique_ptr<Application>(new LabelApplication());
    }
}

void LabelApplication::load()
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

    _ctx.getGorn().getTextures().getDefinitions().get("grid_font.png")
        .withParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST)
        .withParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    auto& loader = _ctx.getLabels().getAtlases()
        .addDefaultLoader<mugato::GridFontAtlasLoader>(
        _ctx.getGorn().getMaterials(), glm::vec2(8.0f));
    loader.setAdvanceDifference(-3.0f);

    _ctx.setScreenSize(glm::vec2(200.0f, 200.0f));

    _label1 = _ctx.getLabels().load("font.fnt");
    _label1.setText("This is a fnt label! gg_jj");

    _label2 = _ctx.getLabels().load("grid_font.png");
    _label2.setText("This is a grid label!");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void LabelApplication::update(double dt)
{
    _label1.update(dt);
    _label2.update(dt);
}

void LabelApplication::draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    _ctx.getGorn().getQueue().addCommand()
        .withTransform(
            glm::translate(glm::mat4(),
            glm::vec3(25.f, 25.f, 0.0f)));
    _label1.render(_ctx.getGorn().getQueue());
    _ctx.getGorn().getQueue().addCommand()
        .withTransform(
            glm::translate(glm::mat4(),
            glm::vec3(25.f, 50.f, 0.0f)));
    _label2.render(_ctx.getGorn().getQueue());

	_ctx.getGorn().getQueue().draw();
}

