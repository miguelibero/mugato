
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <glm/gtc/constants.hpp>
#include <functional>
#include <random>

class SpineApplication : public mugato::Application
{
public:

    SpineApplication();

    void load() override;

};

namespace gorn
{
    std::unique_ptr<Application> main()
    {
        return std::unique_ptr<Application>(new SpineApplication());
    }
}

SpineApplication::SpineApplication()
{
}

void SpineApplication::load()
{
    mugato::Application::load();

#ifdef GORN_PLATFORM_LINUX
	getGorn().getFiles()
        .makeDefaultLoader<gorn::LocalFileLoader>("../assets/%s");
	getGorn().getImages()
        .makeDefaultDataLoader<gorn::PngImageLoader>();
#elif GORN_PLATFORM_ANDROID
	getGorn().getFiles()
        .makeDefaultLoader<gorn::AssetFileLoader>("%s");
	getGorn().getImages()
        .makeDefaultDataLoader<gorn::GraphicsImageLoader>();
#endif

    getMugato().setScreenSize(glm::vec2(480.0f, 320.0f));

    auto scene = getMugato().getScenes().push();
    
    auto& info = scene->addComponent<mugato::RenderInfoComponent>();
    info.getTransform().setPosition(glm::vec3(0.0f, 60.f, 1.0f));

    auto spineboy = scene->addChild();
    spineboy->addComponent<mugato::SpineSkeletonComponent>("spineboy.json");
    spineboy->getTransform().setPosition(glm::vec2(50.0f));

}
