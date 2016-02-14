
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

class RaycastApplication : public mugato::Application
{
private:	
	void spawnEnemy(mugato::Entity& parent, const glm::vec3& position);
	void moveEnemy(std::shared_ptr<mugato::Entity> unit);
public:
	RaycastApplication();
    void load() override;
	void draw() override;
};

namespace gorn
{
    std::unique_ptr<Application> main()
    {
        return std::unique_ptr<Application>(new RaycastApplication());
    }
}

RaycastApplication::RaycastApplication()
{
	setSize(glm::vec2(800.0f, 600.0f));
}

void RaycastApplication::load()
{
	mugato::Application::load();

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
		.withUniformValue(gorn::UniformKind::Color,
			glm::vec3(0.0f, 1.0f, 0.0f))
		.withTexture(gorn::UniformKind::Texture0, "white")
		.withProgram(mugato::ProgramKind::Light));

	matdefs.set("unit", gorn::MaterialDefinition()
		.withUniformValue(gorn::UniformKind::Color,
			glm::vec3(1.0f, 0.0f, 0.0f))
		.withTexture(gorn::UniformKind::Texture0, "wooden-crate.jpg")
		.withProgram(mugato::ProgramKind::Light));

	getMugato().setViewportSize(getSize());

	auto scene = getMugato().getScenes().push();

	scene->addComponent<mugato::RenderInfoComponent>();

	auto scene3d = scene->addChild();
	auto& cam3d = scene3d->addComponent<mugato::CameraComponent>();

	getGorn().getQueue().setProjectionTransform(
		glm::perspective(
			glm::radians(45.0f), getSize().x / getSize().y, 0.1f, 100.0f));
	getGorn().getQueue().setViewTransform(
		glm::lookAt(
			glm::vec3(0.0f, 10.0f, 10.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
			));

	auto& light = getMugato().getLighting().add(mugato::LightType::Directional);
	light.position = glm::vec3(2, 2, 2);
	light.color = glm::vec3(1.0);
	light.attenuation = 0.1f;
	light.ambientCoefficient = 0.06f;
	light.direction = glm::vec3(-1, -1, -1);

	auto& pointLight = getMugato().getLighting().add(mugato::LightType::Point);
	pointLight.position = glm::vec3(0, 5, 0);
	pointLight.attenuation = 0.1f;
	pointLight.ambientCoefficient = 0.06f;

	auto floorShape = gorn::PlaneShape(
		glm::vec3(10.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 10.0f),
		glm::vec3(0.0f, 0.0f, 0.0f)
	);
	auto floorMesh = std::make_shared<gorn::Mesh>(gorn::ShapeMeshFactory::create(floorShape));
	auto floor = scene3d->addChild();
	floor->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	floor->addComponent<mugato::MeshComponent>(floorMesh, "floor");

	spawnEnemy(*scene3d, glm::vec3(0, 0, 0));
	spawnEnemy(*scene3d, glm::vec3(2, 0, 0));
	spawnEnemy(*scene3d, glm::vec3(-2, 0, 0));
	spawnEnemy(*scene3d, glm::vec3(0, 0, 2));

	gorn::StateChange()
		.withEnable(gorn::TestType::Depth)
		.apply();
}

void RaycastApplication::spawnEnemy(mugato::Entity& parent, const glm::vec3& position)
{
	auto unit = parent.addChild();
	auto unitShape = gorn::Rect(glm::vec3(0.0f), glm::vec3(1.0f));
	auto unitMesh = std::make_shared<gorn::Mesh>(gorn::ShapeMeshFactory::create(unitShape));
	unit->addComponent<mugato::MeshComponent>(unitMesh, "unit");
	unit->getTransform().setPosition(position);
	unit->getTransform().setPivot(glm::vec3(0.5f));
	
	moveEnemy(unit);
}

void RaycastApplication::moveEnemy(std::shared_ptr<mugato::Entity> unit)
{
	auto& action = unit->addComponent<mugato::ActionComponent>();
	unit->getTransform().setRotation(0.0f);
	auto endt = unit->getTransform();
	endt.setRotation(glm::vec3(0.0f, 0.0f, 2 * glm::pi<float>()));
	action.add<mugato::TweenAction>(5.0f, endt)
		.withComplete(std::bind(
			&RaycastApplication::moveEnemy, this, unit));
}

void RaycastApplication::draw()
{
	Application::draw();
}
