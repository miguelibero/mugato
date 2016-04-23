
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
        mugato::EntityTouchPhase phase,
        const glm::vec3& p);

    void createButton(mugato::Entity& p, mugato::Alignment a);
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
#if GORN_PLATFORM_LINUX || GORN_PLATFORM_WINDOWS
	getGorn().getFiles()
        .makeDefaultLoader<gorn::LocalFileLoader>("../assets/%s");
#elif GORN_PLATFORM_ANDROID
	getGorn().getFiles()
        .makeDefaultLoader<gorn::AssetFileLoader>("%s");
#endif
	getGorn().getImages()
        .makeDefaultDataLoader<gorn::StbImageLoader>();

    getGorn().getQueue().addCamera()
		.withProjection(glm::ortho(0.0f, 480.0f, 0.0f, 320.0f));
	getMugato().getRoot().getTransform().setSize(glm::vec2(480.0f, 320.0f));

    getGorn().getMaterials().getDefinitions().get("button")
        .withTexture(gorn::UniformKind::Texture0, "button1.png");

    getGorn().getMaterials().getDefinitions().get("button_pressed")
        .withTexture(gorn::UniformKind::Texture0, "button1.png")
        .withUniformValue(gorn::UniformKind::Color, glm::vec4(100.f/255.f));

    getMugato().getSprites().getDefinitions().get("button")
        .withMaterial("button")
        .withStretchBorders(glm::vec4(4.0f, 5.0f, 9.0f, 5.0f));

    auto scene = getMugato().getScenes().push();
    auto& info = scene->addComponent<mugato::RenderInfoComponent>();

    auto& materials = getGorn().getMaterials();

    createButton(*scene, mugato::Alignment::TopLeft);
	createButton(*scene, mugato::Alignment::Top);
	createButton(*scene, mugato::Alignment::TopRight);
	createButton(*scene, mugato::Alignment::Left);
    createButton(*scene, mugato::Alignment::Center);
	createButton(*scene, mugato::Alignment::Right);
	createButton(*scene, mugato::Alignment::BottomLeft);
	createButton(*scene, mugato::Alignment::Bottom);
	createButton(*scene, mugato::Alignment::BottomRight);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GuiApplication::createButton(mugato::Entity& p, mugato::Alignment a)
{
    auto& materials = getGorn().getMaterials();
    auto buttonEntity = p.addChild();
    auto& button = buttonEntity->addComponent<mugato::ButtonComponent>();

    button.setBackground("button");
    button.setLabel("font.fnt");
    button.setText("This is\na button");
    button.setBackgroundMaterial(
        materials.load("button_pressed"), mugato::ButtonState::Pressed);
    button.setTouchCallback(
        std::bind(&GuiApplication::onButtonTouched,
            this, std::ref(button.getBackground()),
            std::placeholders::_1,
            std::placeholders::_2));

    buttonEntity->getTransform().setPosition(glm::vec2(240, 100));
    buttonEntity->getTransform().setSize(glm::vec2(200, 50));
    buttonEntity->addComponent<mugato::AlignComponent>(a);
}

bool GuiApplication::onButtonTouched(mugato::Sprite& sprite,
    mugato::EntityTouchPhase phase, const glm::vec3& p)
{
    std::string phaseStr;
    switch(phase)
    {
        case mugato::EntityTouchPhase::Begin:
            phaseStr = "begin";
            break;
        case mugato::EntityTouchPhase::Move:
            phaseStr = "move";
            break;
        case mugato::EntityTouchPhase::Cancel:
            phaseStr = "cancel";
            break;
        case mugato::EntityTouchPhase::End:
            phaseStr = "end";
            break;
        case mugato::EntityTouchPhase::None:
            phaseStr = "none";
            break;
    }

    std::cout << "touch " << phaseStr << " "<< &sprite
        << " " << p.x << "," << p.y << std::endl;

    return true;
}
