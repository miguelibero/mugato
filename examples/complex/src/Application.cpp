
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

class ComplexApplication : public mugato::Application
{
private:
    void rotateEntity(std::shared_ptr<mugato::Entity> entity);
public:
    void load() override;
};

namespace gorn
{
    std::unique_ptr<Application> main()
    {
        return std::unique_ptr<Application>(new ComplexApplication());
    }
}

void ComplexApplication::load()
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

    getGorn().getTextures().getDefinitions().get("white")
        .withImage(gorn::Image::createWithColor(glm::vec4(1.0, 1.0, 1.0, 1.0)));

    auto& matdefs = getGorn().getMaterials().getDefinitions();
    matdefs.set("floor", gorn::MaterialDefinition()
        .withUniformValue(gorn::UniformType::Color,
            glm::vec3(1.0f, 1.0f, 1.0f))
        .withTexture(gorn::UniformType::DiffuseTexture, "white")
        .withProgram(mugato::ProgramKind::Light));

	getGorn().getQueue().addCamera()
		.withLayer(2)
		.withProjection(glm::perspective(
			glm::radians(45.0f), getSize().x / getSize().y, 0.1f, 100.0f))
		.withView(glm::lookAt(
			glm::vec3(0.0f, 10.0f, 10.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)));

	getGorn().getQueue().addCamera()
		.withLayer(1)
		.withProjection(glm::ortho(0.0f, getSize().x, 0.0f, getSize().y));

	auto scene = getMugato().getScenes().push();

	auto scene3d = scene->addChild();
	scene3d->setLayer(2);

	auto scene2d = scene->addChild();
	scene2d->setLayer(1);
	scene2d->getTransform().setSize(getSize());
	scene2d->addComponent<mugato::RenderInfoComponent>();

    getMugato().getLighting().add(mugato::LightType::Directional)
        .withLayer(2)
        .withPosition(glm::vec3(-1, 1, 0))
        .withColor(glm::vec3(0.5f));

    auto& progDefs = getGorn().getPrograms().getDefinitions();
    progDefs.set("assimp", [&progDefs](const std::string& name) {
        return progDefs.get(mugato::ProgramKind::Light);
    });

    auto& matDefs = getGorn().getMaterials().getDefinitions();
    matDefs.set("assimp", [&matDefs](const std::string& name) {
        return matDefs.get("floor");
    });

    getGorn().getFiles().addFilter("assimp", [](std::string& name) {
        auto pos = name.find("..\\..\\..\\..\\models\\Collada\\");
        if(pos == 0)
        {
            name = name.substr(27);
        }
    });

	auto buffer = getGorn().getFiles().load("duck.fbx").get();
	auto loader = mugato::AssimpEntityLoader(getGorn().getMaterials());
	auto entity = loader.load(buffer);
    entity->getTransform().setScale(0.03f);
    entity->getTransform().setPositionZ(3.0f);
	scene3d->addChild(entity);
    rotateEntity(entity);

    auto floorShape = gorn::PlaneShape(
        glm::vec3(10.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 10.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)
    );
    auto floorMesh = std::make_shared<gorn::Mesh>(gorn::ShapeMeshFactory::create(floorShape));
    auto floor = scene3d->addChild();
    floor->addComponent<mugato::MeshComponent>(floorMesh, "floor");

    gorn::Capabilities()
        .with(gorn::CapabilityType::DepthTest, true)
        .withLineWidth(1.0f)
        .apply();
}

void ComplexApplication::rotateEntity(std::shared_ptr<mugato::Entity> entity)
{
    entity->getTransform().setRotation(0.0f);
    auto endt = entity->getTransform();
    endt.setRotation(glm::vec3(0.0f, 0.0f, 2 * glm::pi<float>()));
    entity->addAction<mugato::TweenAction>(5.0f, endt)
        .withComplete(std::bind(
            &ComplexApplication::rotateEntity, this, entity));
}