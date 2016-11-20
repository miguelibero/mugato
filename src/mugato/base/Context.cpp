#include <mugato/base/Context.hpp>
#include <mugato/base/Kinds.hpp>
#include <mugato/base/Light.hpp>
#include <mugato/sprite/MaterialSpriteAtlasLoader.hpp>
#include <mugato/sprite/CocosSpriteAtlasLoader.hpp>
#include <mugato/sprite/GdxSpriteAtlasLoader.hpp>
#include <mugato/sprite/StarlingParticleLoader.hpp>
#include <mugato/label/FntFontAtlasLoader.hpp>
#include <mugato/label/DebugFontAtlasConfigurator.hpp>
#include <mugato/scene/EntityStack.hpp>
#include <gorn/gl/ProgramManager.hpp>
#include <gorn/asset/FileManager.hpp>
#include <gorn/platform/StbImageLoader.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace mugato
{

    Context::Context():
    _sprites(_gorn.getMaterials(), _gorn.getFiles()),
	_particles(_sprites, _gorn.getFiles()),
    _labels(_gorn.getMaterials(), _gorn.getFiles()),
    _skeletons(_gorn.getMaterials(), _gorn.getFiles()),
    _fixedUpdateInterval(0.0),
    _fixedUpdatesPerSecond(10.0),
	_touching(false)
    {
        _sprites.getAtlases().makeDefaultDataLoader
            <GdxSpriteAtlasLoader>();
        _sprites.getAtlases().makeDefaultDataLoader
            <CocosSpriteAtlasLoader>();
        _labels.getAtlases().makeDefaultDataLoader
            <FntFontAtlasLoader>();
        _sprites.getAtlases().makeDefaultLoader
            <MaterialSpriteAtlasLoader>(_gorn.getMaterials());
        _gorn.getImages().makeDefaultDataLoader
            <gorn::StbImageLoader>();
		_particles.getConfigs().makeDefaultDataLoader
			<StarlingParticleLoader>();
        _gorn.getMaterials().getDefinitions().set(
            [](const std::string& name){
                return gorn::MaterialDefinition()
                    .withTexture(gorn::UniformType::DiffuseTexture, name)
                    .withProgram(ProgramKind::Sprite);
            });

		_gorn.getPrograms().getDefinitions().get(ProgramKind::Sprite)
			.withShaderData(gorn::ShaderType::Vertex, R"(#version 100
precision highp float;

attribute vec3 position;
attribute vec2 texCoords;
attribute vec4 color;

uniform mat4 model;
uniform mat4 cam;

varying vec2 TexCoords;
varying vec4 Color;

void main()
{
    TexCoords = texCoords;
	Color = color;
    gl_Position = cam * model * vec4(position, 1.0);
}

)")
.withShaderData(gorn::ShaderType::Fragment, R"(#version 100
precision highp float;

varying vec2 TexCoords;
varying vec4 Color;

uniform sampler2D texture;

void main()
{
    gl_FragColor = Color * texture2D(texture, TexCoords);
}

)")
            .withUniform(gorn::UniformKind("model", gorn::UniformType::ModelTransform))
			.withUniform(gorn::UniformKind("cam", gorn::UniformType::CameraTransform))
			.withUniform(gorn::UniformKind("texture", gorn::UniformType::DiffuseTexture))
            .withAttribute(gorn::ProgramAttributeDefinition(
				gorn::AttributeKind("position", gorn::AttributeType::Position))
				.withBasicType(gorn::BasicType::Float)
				.withCount(3))
            .withAttribute(gorn::ProgramAttributeDefinition(
				gorn::AttributeKind("texCoords", gorn::AttributeType::TexCoords))
				.withBasicType(gorn::BasicType::Float)
				.withCount(2))
			.withAttribute(gorn::ProgramAttributeDefinition(
				gorn::AttributeKind("color", gorn::AttributeType::Color))
				.withBasicType(gorn::BasicType::Float)
				.withCount(4)
				.withDefaultValue(buffer{ 1.0f, 1.0f, 1.0f, 1.0f }));

		_gorn.getPrograms().getDefinitions().get(ProgramKind::Color)
            .withShaderData(gorn::ShaderType::Vertex, R"(#version 100

precision highp float;

attribute vec3 position;

uniform mat4 model;
uniform mat4 cam;
uniform vec4 color;

void main()
{
    gl_Position = cam * model * vec4(position, 1.0);
}

)")
            .withShaderData(gorn::ShaderType::Fragment, R"(#version 100

precision highp float;

uniform vec4 color;

void main()
{
    gl_FragColor = color;
}

)")
			.withUniform(gorn::UniformKind("model", gorn::UniformType::ModelTransform))
			.withUniform(gorn::UniformKind("cam", gorn::UniformType::CameraTransform))
			.withUniform(gorn::UniformKind("color", gorn::UniformType::Color))
			.withAttribute(gorn::ProgramAttributeDefinition(
				gorn::AttributeKind("position", gorn::AttributeType::Position))
				.withBasicType(gorn::BasicType::Float)
				.withCount(3));

        DebugFontAtlasConfigurator().setup(*this);

		_lighting.setup(*this);

        _root = std::make_shared<Entity>();
        _root->setContext(*this);
        _scenes = &_root->addComponent<EntityStack>();
    }

    const gorn::Context& Context::getGorn() const
    {
        return _gorn;
    }

    gorn::Context& Context::getGorn()
    {
        return _gorn;
    }

    const SpriteManager& Context::getSprites() const
    {
        return _sprites;
    }

    SpriteManager& Context::getSprites()
    {
        return _sprites;
    }

    const LabelManager& Context::getLabels() const
    {
        return _labels;
    }

    LabelManager& Context::getLabels()
    {
        return _labels;
    }

    const SpineManager& Context::getSkeletons() const
    {
        return _skeletons;
    }

    SpineManager& Context::getSkeletons()
    {
        return _skeletons;
    }

	const ParticleManager& Context::getParticles() const
	{
		return _particles;
	}

	ParticleManager& Context::getParticles()
	{
		return _particles;
	}

    const EntityStack& Context::getScenes() const
    {
        return *_scenes;
    }

    EntityStack& Context::getScenes()
    {
        return *_scenes;
    }

	const LightingSystem& Context::getLighting() const
	{
		return _lighting;
	}

	LightingSystem& Context::getLighting()
	{
		return _lighting;
	}

    const Entity& Context::getRoot() const
    {
        return *_root;
    }

    Entity& Context::getRoot()
    {
        return *_root;
    }

    void Context::setFixedUpdatesPerSecond(double fps)
    {
        _fixedUpdatesPerSecond = fps;
    }

    void Context::update(double dt)
    {
        _fixedUpdateInterval += dt;
        auto fixedUpdateDuration = 1.0/_fixedUpdatesPerSecond;
        if(_fixedUpdateInterval > fixedUpdateDuration)
        {
            fixedUpdate(_fixedUpdateInterval);
            while(_fixedUpdateInterval > fixedUpdateDuration)
            {
                _fixedUpdateInterval -= fixedUpdateDuration;
            }
        }
        _root->update(dt);
        _gorn.getQueue().update(dt);
    }

    void Context::fixedUpdate(double dt)
    {
        _root->fixedUpdate(dt);
    }

    void Context::draw()
    {
		_lighting.render(_gorn.getQueue());
		_root->render(_gorn.getQueue());
        _gorn.getQueue().draw();
    }

    void Context::touch(const glm::vec2& p)
    {
		auto phase = EntityTouchPhase::Begin;
		if(_touching)
		{
			phase = EntityTouchPhase::Move;
		}
		else
		{
			_touching = true;
		}
		doTouch(p, phase);
    }

    void Context::touchEnd(const glm::vec2& p)
    {
		_touching = false;
		doTouch(p, EntityTouchPhase::End);
    }

	void Context::doTouch(const glm::vec2& p, Entity::TouchPhase phase)
	{
		for(auto& cam : _gorn.getQueue().getCameras())
		{
			_root->onScreenTouched(*cam, p, phase);
		}
	}

}
