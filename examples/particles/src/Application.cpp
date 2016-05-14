
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

class ParticlesAnimation : public gorn::Application
{
	mugato::Context _ctx;
    mugato::Sprite _sprite1;
    mugato::Sprite _sprite2;
	gorn::Mesh _debugMesh;

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
	_ctx.getGorn().getFiles()
        .makeDefaultLoader<gorn::LocalFileLoader>("../assets/%s");
#elif GORN_PLATFORM_ANDROID
	_ctx.getGorn().getFiles()
        .makeDefaultLoader<gorn::AssetFileLoader>("%s");
#endif
	_ctx.getGorn().getImages()
        .makeDefaultDataLoader<gorn::StbImageLoader>();

	
}

