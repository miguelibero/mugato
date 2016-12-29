
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>

class LightingApplication : public mugato::Application
{
private:
	void spawnUnit(mugato::Entity& parent, const glm::vec3& position);
	void moveUnit(std::shared_ptr<mugato::Entity> unit);
	void moveLight(std::shared_ptr<mugato::Entity> light, bool dir);
public:
	LightingApplication();
    void load() override;
};

namespace gorn
{
    std::unique_ptr<Application> main()
    {
        return std::unique_ptr<Application>(new LightingApplication());
    }
}

LightingApplication::LightingApplication()
{
	setSize(glm::vec2(800.0f, 600.0f));
}

void LightingApplication::load()
{
	mugato::Application::load();

	//getGorn().getQueue().setBatching(false);

#if GORN_PLATFORM_LINUX || GORN_PLATFORM_WINDOWS
	getGorn().getFiles()
		.makeDefaultLoader<gorn::LocalFileLoader>("../assets/%s");
#elif GORN_PLATFORM_ANDROID
	getGorn().getFiles()
		.makeDefaultLoader<gorn::AssetFileLoader>("%s");
#endif

	getGorn().getTextures().getDefinitions().get("white")
		.withImage(gorn::Image::createWithColor(glm::vec4(1.0, 1.0, 1.0, 1.0)));

  auto& matdefs = getGorn().getMaterials().getDefinitions();
	matdefs.set("floor", gorn::MaterialDefinition()
		.withUniformValue(gorn::UniformType::Color,
			glm::vec3(1.0f, 1.0f, 1.0f))
		.withTexture(gorn::UniformType::DiffuseTexture, "white")
		.withProgram(mugato::ProgramKind::Light));

	matdefs.set("unit", gorn::MaterialDefinition()
		.withUniformValue(gorn::UniformType::Color,
			glm::vec3(1.0f, 1.0f, 1.0f))
		.withTexture(gorn::UniformType::DiffuseTexture, "wooden-crate.png")
		.withProgram(mugato::ProgramKind::Light));

	matdefs.set("normal", gorn::MaterialDefinition()
		.withUniformValue(gorn::UniformType::Color,
			glm::vec4(0.0f, 1.0f, 0.0f, 1.0f))
		.withProgram(mugato::ProgramKind::Color));

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

	auto pointLight = scene3d->addChild();
	pointLight->setLayer(2);
	pointLight->getTransform().setPosition(glm::vec3(0, 5, 0));
	moveLight(pointLight, true);

	pointLight->addComponent<mugato::LightComponent>()
		.withAttenuation(0.01f)
		.withDirection(glm::vec3(0, -1, 0))
		.withConeAngle(20)
		.withAmbient(0.2f);

	auto size = glm::vec2(5.0f, 5.0f);

	auto floorShape = gorn::PlaneShape(
		glm::vec3(size.x * 2.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, size.y * 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f)
	);
	auto floorMesh = std::make_shared<gorn::Mesh>(gorn::ShapeMeshFactory::create(floorShape));
	auto floor = scene3d->addChild();
	floor->addComponent<mugato::MeshComponent>(floorMesh, "floor");
	floor->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	auto sep = 2.0f;
	for(auto x = -size.x; x < size.x; x += sep)
	{
		for(auto y = -size.y; y < size.y; y += sep)
		{
			spawnUnit(*scene3d, glm::vec3(x, 0, y));
		}
	}

	gorn::Capabilities()
		.with(gorn::CapabilityType::DepthTest, true)
		.withLineWidth(1.0f)
		.apply();
}

void LightingApplication::spawnUnit(mugato::Entity& parent, const glm::vec3& position)
{
	auto unit = parent.addChild();
	auto size = glm::vec3(1.0f);
	auto unitShape = gorn::Rect(glm::vec3(0.0f), size);
	auto unitMesh = std::make_shared<gorn::Mesh>(gorn::ShapeMeshFactory::create(unitShape));
	unit->addComponent<mugato::MeshComponent>(unitMesh, "unit");
	auto pivot = glm::vec3(0.5f, 0.0f, 0.5f);
	unit->getTransform().setPosition(position+pivot);
	unit->getTransform().setPivot(pivot);
	unit->getTransform().setSize(size);

	moveUnit(unit);
}

void LightingApplication::moveUnit(std::shared_ptr<mugato::Entity> unit)
{
	unit->getTransform().setRotation(0.0f);
	auto endt = unit->getTransform();
	endt.setRotation(glm::vec3(0.0f, 0.0f, 2 * glm::pi<float>()));
	unit->addAction<mugato::TweenAction>(5.0f, endt)
		.withComplete(std::bind(
			&LightingApplication::moveUnit, this, unit));
}

void LightingApplication::moveLight(std::shared_ptr<mugato::Entity> light, bool dir)
{
	auto pos = light->getTransform().getPosition();
	pos.x = 5.0f;
	pos.x *= dir ? -1.0f : 1.0f;
	light->getTransform().setPosition(pos);
	auto endt = light->getTransform();
	pos.x *= -1.0f;
	endt.setPosition(pos);
	light->addAction<mugato::TweenAction>(5.0f, endt)
		.withComplete(std::bind(
			&LightingApplication::moveLight, this, light, !dir));
}
