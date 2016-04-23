
#include <gorn/gorn.hpp>
#include <mugato/mugato.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/constants.hpp>
#include <random>

class ShooterApplication : public mugato::Application
{
private:
	std::shared_ptr<mugato::Entity> _scene3d;
	std::shared_ptr<mugato::Entity> _player;
	std::vector<glm::ivec2> _enemies;
	std::mt19937 _randomAlgo;
	std::uniform_real_distribution<float> _randomPosDistri;

	void defineAssets();
	void createRenderInfo(const std::shared_ptr<mugato::Entity>& scene);
	void spawnEnemy();
	void touch(const glm::vec2& p) override;
	bool onEnemyTouched(mugato::Entity& entity, const glm::vec3& p,
		mugato::Entity::TouchPhase phase);
	float randomPos();
public:
	ShooterApplication();
    void load() override;
};

namespace gorn
{
    std::unique_ptr<Application> main()
    {
        return std::unique_ptr<Application>(new ShooterApplication());
    }
}

ShooterApplication::ShooterApplication():
_randomPosDistri(-10.0f,10.0f)
{
	std::random_device rd;
	_randomAlgo = decltype(_randomAlgo)(rd());

	setSize(glm::vec2(800.0f, 600.0f));
}

float ShooterApplication::randomPos()
{
	return _randomPosDistri(_randomAlgo);
}

void ShooterApplication::defineAssets()
{
	getGorn().getTextures().getDefinitions().get("green")
		.withImage(gorn::Image::createWithColor(glm::vec4(0.0, 1.0, 0.0, 1.0)));

	getGorn().getTextures().getDefinitions().get("red")
		.withImage(gorn::Image::createWithColor(glm::vec4(1.0, 0.0, 0.0, 1.0)));

	getGorn().getTextures().getDefinitions().get("blue")
		.withImage(gorn::Image::createWithColor(glm::vec4(0.5, 0.5, 1.0, 1.0)));

	getGorn().getTextures().getDefinitions().get("yellow")
		.withImage(gorn::Image::createWithColor(glm::vec4(1.0, 1.0, 0.5, 1.0)));

	auto& matdefs = getGorn().getMaterials().getDefinitions();
	matdefs.set("player", gorn::MaterialDefinition()
		.withUniformValue(gorn::UniformKind::Color,
			glm::vec3(1.0f, 0.0f, 1.0f))
		.withTexture(gorn::UniformKind::Texture0, "green")
		.withProgram(mugato::ProgramKind::Light));

	matdefs.set("enemy", gorn::MaterialDefinition()
		.withUniformValue(gorn::UniformKind::Color,
			glm::vec3(1.0f, 1.0f, 1.0f))
		.withTexture(gorn::UniformKind::Texture0, "blue")
		.withProgram(mugato::ProgramKind::Light));

	matdefs.set("floor", gorn::MaterialDefinition()
		.withUniformValue(gorn::UniformKind::Color,
			glm::vec3(1.0f, 1.0f, 1.0f))
		.withTexture(gorn::UniformKind::Texture0, "yellow")
		.withProgram(mugato::ProgramKind::Light));

	matdefs.set("ray", gorn::MaterialDefinition()
		.withUniformValue(gorn::UniformKind::Color,
			glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))
		.withProgram(mugato::ProgramKind::Color));
}

void ShooterApplication::createRenderInfo(const std::shared_ptr<mugato::Entity>& scene)
{
	getGorn().getQueue().addCamera()
		.withLayer(1)
		.withProjection(glm::ortho(0.0f, getSize().x, 0.0f, getSize().y));

	auto scene2d = scene->addChild();
	scene2d->setLayer(1);
	scene2d->getTransform().setSize(getSize());
	scene2d->addComponent<mugato::RenderInfoComponent>();
}

void ShooterApplication::load()
{
	mugato::Application::load();
	defineAssets();
	auto scene = getMugato().getScenes().push();
	createRenderInfo(scene);

	getMugato().getLighting().add(mugato::LightType::Directional)
		.withLayer(2)
		.withPosition(glm::vec3(1, 1, 0))
		.withColor(glm::vec3(1.0f))
		.withAmbient(0.2f);

	_scene3d = scene->addChild();
	_scene3d->setLayer(2);

	_player = _scene3d->addChild();
	auto playerShape = gorn::Rect(glm::vec3(0.0f), glm::vec3(0.5f, 1.0f, 1.0f));
	auto playerMesh = std::make_shared<gorn::Mesh>(
		gorn::ShapeMeshFactory::create(playerShape));
	_player->getTransform().setSize(playerShape.size);
	_player->getTransform().setPivot(glm::vec3(-0.25f, 0.0f, -0.5f));
	_player->addComponent<mugato::MeshComponent>(playerMesh, "player");
	
	auto playerCam = _player->addChild();
	playerCam->setLayer(2);
	playerCam->getTransform().setPosition(glm::vec3(1.5f, 1.5f, 4.0f));
	playerCam->addComponent<mugato::CameraComponent>()
		.withProjection(glm::perspective(
			glm::radians(45.0f), getSize().x / getSize().y, 0.1f, 100.0f))
		.withLookAt(glm::vec3(1.5f, 0.0f, 0.0f));

	auto floorShape = gorn::PlaneShape(
		glm::vec3(10.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 10.0f),
		glm::vec3(0.0f, 0.0f, 0.0f)
		);
	auto floorMesh = std::make_shared<gorn::Mesh>(gorn::ShapeMeshFactory::create(floorShape));
	auto floor = _scene3d->addChild();
	floor->addComponent<mugato::MeshComponent>(floorMesh, "floor");
	floor->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	for(int i = 0; i < 5; i++)
	{
		spawnEnemy();
	}

	gorn::StateChange()
		.withEnable(gorn::TestType::Depth)
		.apply();
	glLineWidth(3.0f);
}

bool ShooterApplication::onEnemyTouched(mugato::Entity& entity, const glm::vec3& p,
	mugato::Entity::TouchPhase phase)
{
	if(phase == mugato::Entity::TouchPhase::End)
	{
		auto ray = _scene3d->addChild();
		auto playerPos = _player->getTransform().getPosition();
		auto enemyPos = entity.getTransform().getLocalToParentPoint(p);
		auto rayShape = gorn::Ray(
			playerPos, enemyPos - playerPos);
		auto rayMesh = std::make_shared<gorn::Mesh>(
			gorn::ShapeMeshFactory::create(rayShape, gorn::DrawMode::Lines));
		ray->addComponent<mugato::MeshComponent>(rayMesh, "ray");
		auto& actions = ray->addAction<mugato::SequenceAction>(1.0f);
		actions.add<mugato::EmptyAction>(1.0f);
		actions.add<mugato::RemoveAction>(0.0f);
	}
	return true;
}

void ShooterApplication::touch(const glm::vec2& p)
{
	mugato::Application::touch(p);

	float speed = 0.4f;
	glm::vec2 d(speed*p);
	auto& t = _player->getTransform();
	t.setPosition(t.getPosition() + glm::vec3(d.x, 0.0f, -d.y));
}

void ShooterApplication::spawnEnemy()
{
	auto enemy = _scene3d->addChild();
	auto enemyShape = gorn::Rect(glm::vec3(0.0f), glm::vec3(0.5f));
	auto enemyMesh = std::make_shared<gorn::Mesh>(
		gorn::ShapeMeshFactory::create(enemyShape));
	enemy->getTransform().setSize(enemyShape.size);
	enemy->getTransform().setPosition(glm::vec3(
		randomPos()*enemyShape.size.x, 0.0f, randomPos()*enemyShape.size.z));
	enemy->addComponent<mugato::MeshComponent>(enemyMesh, "enemy");
	enemy->addComponent<mugato::TouchComponent>(
		std::bind(&ShooterApplication::onEnemyTouched, this,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}
