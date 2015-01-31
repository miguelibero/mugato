
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

namespace gorn
{
	mugato::Context _ctx;
    mugato::Sprite _sprite1;
    mugato::Sprite _sprite2;

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

        _ctx.getGorn().getTextures().getDefinitions().get("guybrush.png")
            .withParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST)
            .withParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        _ctx.getSprites().getDefinitions().get("guybrush")
            .withAtlas("guybrush.atlas")
            .withAnimation("walk", mugato::SpriteAnimationDefinition()
                .withFrames("gb_walk")
                .withFrameDuration(1.0f/5.0f));

        _ctx.setScreenSize(glm::vec2(200.0f, 200.0f));

        _sprite1 = _ctx.getSprites().load("guybrush");
        _sprite1.play("walk");

        _sprite2 = _ctx.getSprites().load("guybrush");
        _sprite2.play("walk");
	}

	void Application::unload()
	{
	}

	void Application::update(double dt)
	{
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        _sprite1.update(dt);
        _sprite2.update(dt);

        _sprite1.render(_ctx.getGorn().getQueue());
        _ctx.getGorn().getQueue().addCommand()
            .withTransform(
                glm::translate(glm::mat4(),
                glm::vec3(100.f, 100.f, 0.0f)));
        _sprite2.render(_ctx.getGorn().getQueue());

		_ctx.getGorn().getQueue().draw();
	}

}
