
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>
#include <functional>
#include <iostream>

class GuiApplication : public mugato::Application
{
private:
    bool onButtonTouched(mugato::Sprite& sprite,
        const glm::vec2& p, mugato::EntityTouchPhase phase);
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

    getGorn().getMaterials().getDefinitions().get("button")
        .withTexture(gorn::UniformKind::Texture0, "button1.png");

    getGorn().getMaterials().getDefinitions().get("button_pressed")
        .withTexture(gorn::UniformKind::Texture0, "button1.png")
        .withUniformValue(gorn::UniformKind::Color, glm::vec4(100.f/255.f));

    getMugato().getSprites().getDefinitions().get("button")
        .withMaterial("button")
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

    scene->addComponent<mugato::RenderInfoComponent>();

    auto button = scene->addChild();

    auto& sprite = button->addComponent<mugato::SpriteComponent>("button",
        mugato::SpriteResizeMode::Exact).getSprite();

    auto& label = button->addComponent<mugato::LabelComponent>(
        "This is\na button", "font.fnt");
    label.getLabel().setAlignment(mugato::LabelAlignment::Center);

    button->addComponent<mugato::TouchComponent>(
        std::bind(&GuiApplication::onButtonTouched,
            this, std::ref(sprite),
            std::placeholders::_2,
            std::placeholders::_3));

    button->getTransform().setPosition(glm::vec2(240, 100));
    button->getTransform().setSize(glm::vec2(200, 50));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

bool GuiApplication::onButtonTouched(mugato::Sprite& sprite,
    const glm::vec2& p, mugato::EntityTouchPhase phase)
{
    std::string mat = "button_pressed";
    if(phase == mugato::EntityTouchPhase::End)
    {
        mat = "button";
    }
    else
    {
        std::cout << "button touched " << p.x << "," << p.y << std::endl;
    }
    sprite.setMaterial(getGorn().getMaterials().load(mat));

    return true;
}


