
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>

namespace gorn
{
	mugato::Context _ctx;


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

        auto& scene = _ctx.getScenes().push();
        scene.addComponent<mugato::SpriteComponent>("background.png");
        scene.setPosition(glm::vec2(-1.0f, -1.0f));

        auto& guy = scene.addChild();
        guy.addComponent<mugato::SpriteComponent>("character.png");
        guy.setPosition(glm::vec2(1.0f, 1.0f));
	}

	void Application::unload()
	{
	}

	void Application::update(double dt)
	{
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

       _ctx.update(dt);
       _ctx.render();
	}

}
