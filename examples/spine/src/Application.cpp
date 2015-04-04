
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <glm/gtc/constants.hpp>
#include <functional>
#include <random>
#include <iostream>

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

    auto& matdefs = getGorn().getMaterials().getDefinitions();
    matdefs.set("spine_slots", gorn::MaterialDefinition()
        .withUniformValue(gorn::UniformKind::Color,
            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))
        .withProgram(mugato::ProgramKind::Color));

    matdefs.set("spine_bones", gorn::MaterialDefinition()
        .withUniformValue(gorn::UniformKind::Color,
            glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))
        .withProgram(mugato::ProgramKind::Color));

    auto scene = getMugato().getScenes().push();
    
    auto& info = scene->addComponent<mugato::RenderInfoComponent>();
    info.getTransform().setPosition(glm::vec3(0.0f, 60.f, 1.0f));

    {
        auto raptor = scene->addChild();
        auto& skel = raptor->addComponent<mugato::SpineSkeletonComponent>("raptor");
        raptor->getTransform().setScale(0.3f);
        raptor->getTransform().setPosition(glm::vec2(220.0f, 10.0f));
        skel.getSkeleton().setSlotsMaterial(
            getGorn().getMaterials().load("spine_slots")
        );
        skel.getSkeleton().setBonesMaterial(
            getGorn().getMaterials().load("spine_bones")
        );
        skel.getSkeleton().setAnimation("walk");
    }

    {
        auto spineboy = scene->addChild();
        auto& skel = spineboy->addComponent<mugato::SpineSkeletonComponent>("spineboy");
        spineboy->getTransform().setScale(0.3f);
        spineboy->getTransform().setPosition(glm::vec2(400.0f, 10.0f));
        skel.getSkeleton().setSlotsMaterial(
            getGorn().getMaterials().load("spine_slots")
        );
        skel.getSkeleton().setBonesMaterial(
            getGorn().getMaterials().load("spine_bones")
        );
        skel.getSkeleton().setAnimation("jump");
    }

}
