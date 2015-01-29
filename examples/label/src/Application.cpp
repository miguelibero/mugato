
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

namespace gorn
{
	mugato::Context _ctx;
    mugato::Label _label1;
    mugato::Label _label2;

	Application::Application()
	{
	}

	void Application::load()
	{
#ifdef GORN_PLATFORM_LINUX
		_ctx.getGorn().getFiles()
            .addDefaultLoader<LocalFileLoader>("../assets/%s");
		_ctx.getGorn().getImages()
            .addDefaultDataLoader<PngImageLoader>();
#elif GORN_PLATFORM_ANDROID
		_ctx.getGorn().getFiles()
            .addDefaultLoader<BundleFileLoader>("%s");
		_ctx.getGorn().getImages()
            .addDefaultDataLoader<GraphicsImageLoader>();
#endif

        _ctx.getGorn().getTextures().getDefinitions().get("grid_font.png")
            .withParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST)
            .withParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        auto& loader = _ctx.getLabels().getAtlases()
            .addDefaultLoader<mugato::GridFontAtlasLoader>(
            _ctx.getGorn().getMaterials(), glm::vec2(8.0f));
        loader.setAdvanceDifference(-3.0f);

        _ctx.getGorn().getQueue().setBaseTransform(
                glm::translate(glm::mat4(),
                glm::vec3(-0.75f, -0.25f, 0.0f)));

        _label1 = _ctx.getLabels().load("font.fnt");
        _label1.setText("This is a fnt label!");

        _label2 = _ctx.getLabels().load("grid_font.png");
        _label2.setText("This is a grid label!");

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Application::unload()
	{
	}

	void Application::update(double dt)
	{
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        _label1.update(dt);
        _label2.update(dt);

        _label1.render(_ctx.getGorn().getQueue());
        _ctx.getGorn().getQueue().addCommand()
            .withTransform(
                glm::translate(glm::mat4(),
                glm::vec3(0.0f, 0.5f, 0.0f)));
        _label2.render(_ctx.getGorn().getQueue());

		_ctx.getGorn().getQueue().draw();
	}

}
