
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

namespace gorn
{
	mugato::Context _ctx;
    mugato::Label _label;

	Application::Application()
	{
	}

	void Application::load()
	{
#ifdef GORN_PLATFORM_LINUX
		_ctx.getGorn().getFiles()
            .addDefaultLoader<LocalFileLoader>("../assets/%s");
		_ctx.getGorn().getImages()
            .addDefaultLoader<PngImageLoader>();
#elif GORN_PLATFORM_ANDROID
		_ctx.getGorn().getFiles()
            .addDefaultLoader<BundleFileLoader>("%s");
		_ctx.getGorn().getImages()
            .addDefaultLoader<GraphicsImageLoader>();
#endif

        _ctx.getGorn().getQueue().setBaseTransform(
                glm::translate(glm::mat4(),
                glm::vec3(-0.5f, -0.5f, 0.0f)));

        _label = _ctx.getLabels().load("font.fnt");
        _label.setText("This is a label!");

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

        _label.update(dt);
        _label.render(_ctx.getGorn().getQueue());
		_ctx.getGorn().getQueue().draw();
	}

}
