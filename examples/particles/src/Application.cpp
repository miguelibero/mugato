
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

class ParticlesAnimation : public mugato::Application
{

public:

    void load() override;
};

namespace gorn
{
    std::unique_ptr<Application> main()
    {
        return std::unique_ptr<Application>(new ParticlesAnimation());
    }
}

void ParticlesAnimation::load()
{
#if GORN_PLATFORM_LINUX || GORN_PLATFORM_WINDOWS
	getGorn().getFiles()
        .makeDefaultLoader<gorn::LocalFileLoader>("../assets/%s");
#elif GORN_PLATFORM_ANDROID
	getGorn().getFiles()
        .makeDefaultLoader<gorn::AssetFileLoader>("%s");
#endif
	getGorn().getImages()
        .makeDefaultDataLoader<gorn::StbImageLoader>();

	getMugato().getParticles().getDefinitions()
		.get("fire")
		.withConfig("fire.pex");


	glm::vec2 size(480.0f, 320.0f);
	setSize(size);
	getGorn().getQueue().addCamera()
		.withProjection(glm::ortho(0.0f, size.x, 0.0f, size.y));

	auto scene = getMugato().getScenes().push();
	scene->addComponent<mugato::RenderInfoComponent>();

	auto particles = scene->addChild();
	particles->getTransform().setSize(glm::vec2(240.0f, 160.0f));
	particles->addComponent<mugato::ParticleSystemComponent>("fire");

	
}

