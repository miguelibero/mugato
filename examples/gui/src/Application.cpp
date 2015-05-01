
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

class GuiApplication : public mugato::Application
{
public:
    void load() override;
};

namespace gorn
{
    std::unique_ptr<Application> main()
    {
        return std::unique_ptr<Application>(new GuiApplication());
    }
}

void GuiApplication::load()
{
#ifdef GORN_PLATFORM_LINUX
	getGorn().getFiles()
        .makeDefaultLoader<gorn::LocalFileLoader>("../assets/%s");
#elif GORN_PLATFORM_ANDROID
	getGorn().getFiles()
        .makeDefaultLoader<gorn::AssetFileLoader>("%s");
#endif
	getGorn().getImages()
        .makeDefaultDataLoader<gorn::StbImageLoader>();

    getMugato().setScreenSize(glm::vec2(480.0f, 320.0f));

    getMugato().getSprites().getDefinitions().get("button1")
        .withMaterial("button1.png")
        .withStretchBorders(glm::vec4(4, 5, 9, 5));

    auto& matdefs = getGorn().getMaterials().getDefinitions();
    matdefs.set("octree_elements", gorn::MaterialDefinition()
        .withUniformValue(gorn::UniformKind::Color,
            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))
        .withProgram(mugato::ProgramKind::Color));

    matdefs.set("octree_nodes", gorn::MaterialDefinition()
        .withUniformValue(gorn::UniformKind::Color,
            glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))
        .withProgram(mugato::ProgramKind::Color));
   
    auto scene = getMugato().getScenes().push();

    auto& materials = getGorn().getMaterials();
    auto& octree = scene->addComponent<mugato::OcTreeRenderComponent>();
    octree.setElementsMaterial(materials.load("octree_elements"));
    octree.setElementsDrawMode(gorn::DrawMode::Lines);
    octree.setNodesMaterial(materials.load("octree_nodes"));
    octree.setNodesDrawMode(gorn::DrawMode::Lines);

    auto& debugInfo = scene->addComponent<mugato::RenderInfoComponent>();
    debugInfo.getTransform().setPosition(glm::vec3(0.0f, 60.f, 1.0f));

    auto label = scene->addChildWithComponent
        <mugato::LabelComponent>("Label!", "font.fnt");

    label->getTransform().setPosition(glm::vec2(240, 300));

    auto button = scene->addChildWithComponent
        <mugato::SpriteComponent>("button1",
            mugato::SpriteResizeMode::Exact);

    button->getTransform().setPosition(glm::vec2(240, 100));
    button->getTransform().setSize(glm::vec2(200, 50));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


