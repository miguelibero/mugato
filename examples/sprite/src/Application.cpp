
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

class SpriteApplication : public gorn::Application
{
	mugato::Context _ctx;
    mugato::Sprite _sprite1;
    mugato::Sprite _sprite2;
	gorn::Mesh _debugMesh;

public:

    void load() override;
    void update(double dt) override;
    void draw() override;
};

namespace gorn
{
    std::unique_ptr<Application> main()
    {
        return std::unique_ptr<Application>(new SpriteApplication());
    }
}

void SpriteApplication::load()
{
#if GORN_PLATFORM_LINUX || GORN_PLATFORM_WINDOWS
	_ctx.getGorn().getFiles()
        .makeDefaultLoader<gorn::LocalFileLoader>("../assets/%s");
#elif GORN_PLATFORM_ANDROID
	_ctx.getGorn().getFiles()
        .makeDefaultLoader<gorn::AssetFileLoader>("%s");
#endif
	_ctx.getGorn().getImages()
        .makeDefaultDataLoader<gorn::StbImageLoader>();

	auto& matdefs = _ctx.getGorn().getMaterials().getDefinitions();

	matdefs.set("debug", gorn::MaterialDefinition()
		.withUniformValue(gorn::UniformKind::Color,
			glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))
		.withProgram(mugato::ProgramKind::Color));

    _ctx.getGorn().getTextures().getDefinitions().get("guybrush.png")
        .withParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST)
        .withParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    _ctx.getSprites().getDefinitions().get("guybrush")
        .withAtlas("guybrush.atlas")
        .withAnimation("walk", mugato::SpriteAnimationDefinition()
            .withFrame("gb_walk")
            .withFrameDuration(1.0f/5.0f));

	_ctx.getGorn().getQueue().addCamera()
		.withProjection(glm::ortho(0.0f, 200.0f, 0.0f, 250.0f));

    _sprite1 = _ctx.getSprites().load("guybrush");
    _sprite1.play("walk");

    _sprite2 = _ctx.getSprites().load("guybrush");
    _sprite2.play("walk");

	_debugMesh = gorn::ShapeMeshFactory::create(
		gorn::Rect(glm::vec2(0.0f), glm::vec2(100.0f)).shape());
}

void SpriteApplication::update(double dt)
{
    _ctx.update(dt);
    _sprite1.update(dt);
    _sprite2.update(dt);
}

void SpriteApplication::draw()
{
	_ctx.getGorn().getQueue().addCommand()
		.withClearAction(gorn::ClearAction()
			.withColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
			.withType(gorn::ClearType::Color));

	auto cmd = _debugMesh.render();
	cmd.withMaterial(_ctx.getGorn().getMaterials().load("debug"));
	_ctx.getGorn().getQueue().addCommand(std::move(cmd));

    _sprite1.render(_ctx.getGorn().getQueue());
	_ctx.getGorn().getQueue().addCommand()
        .withTransform(
            glm::translate(glm::mat4(),
            glm::vec3(100.f, 100.f, 0.0f)));
    _sprite2.render(_ctx.getGorn().getQueue());

	_ctx.getGorn().getQueue().draw();
}
