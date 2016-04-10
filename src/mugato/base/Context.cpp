#include <mugato/base/Context.hpp>
#include <mugato/base/Kinds.hpp>
#include <mugato/base/Light.hpp>
#include <mugato/sprite/MaterialSpriteAtlasLoader.hpp>
#include <mugato/sprite/CocosSpriteAtlasLoader.hpp>
#include <mugato/sprite/GdxSpriteAtlasLoader.hpp>
#include <mugato/label/FntFontAtlasLoader.hpp>
#include <mugato/label/DebugFontAtlasConfigurator.hpp>
#include <mugato/scene/EntityStack.hpp>
#include <gorn/gl/ProgramManager.hpp>
#include <gorn/render/RenderKinds.hpp>
#include <gorn/asset/FileManager.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace mugato
{

    Context::Context():
    _sprites(_gorn.getMaterials(), _gorn.getFiles()),
    _labels(_gorn.getMaterials(), _gorn.getFiles()),
    _skeletons(_gorn.getMaterials(), _gorn.getFiles()),
    _fixedUpdateInterval(0.0),
    _fixedUpdatesPerSecond(10.0)
    {
        _sprites.getAtlases().makeDefaultDataLoader
            <GdxSpriteAtlasLoader>();
        _sprites.getAtlases().makeDefaultDataLoader
            <CocosSpriteAtlasLoader>();
        _labels.getAtlases().makeDefaultDataLoader
            <FntFontAtlasLoader>();
        _sprites.getAtlases().makeDefaultLoader
            <MaterialSpriteAtlasLoader>(_gorn.getMaterials());

        _gorn.getMaterials().getDefinitions().set(
            [](const std::string& name){
                return gorn::MaterialDefinition()
                    .withTexture(gorn::UniformKind::Texture0, name)
                    .withProgram(ProgramKind::Sprite);
            });

		_gorn.getPrograms().getDefinitions().get(ProgramKind::Sprite)
			.withShaderData(gorn::ShaderType::Vertex, R"(#version 100
precision highp float;

attribute vec3 position;
attribute vec2 texCoords;

uniform mat4 model;
uniform mat4 cam;
uniform vec4 color;

varying vec2 TexCoords;

void main()
{
    TexCoords = texCoords;
    gl_Position = cam * model * vec4(position, 1.0);
}

)")
.withShaderData(gorn::ShaderType::Fragment, R"(#version 100
precision highp float;

varying vec2 TexCoords;

uniform sampler2D texture;
uniform vec4 color;

void main()
{
    gl_FragColor = color * texture2D(texture, TexCoords);
}

)")
            .withUniform(gorn::UniformKind::Model, "model")
            .withUniform(gorn::UniformKind::Camera, "cam")
            .withUniform(gorn::UniformKind::Texture0, "texture")
            .withUniform(gorn::UniformKind::Color,
                gorn::ProgramUniformDefinition("color")
                .withValue(glm::vec4(1.0f)))
            .withAttribute(gorn::AttributeKind::Position, "position")
            .withAttribute(gorn::AttributeKind::TexCoords, "texCoords");


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
            .withUniform(gorn::UniformKind::Model, "model")
            .withUniform(gorn::UniformKind::Camera, "cam")
            .withUniform(gorn::UniformKind::Color, "color")
            .withAttribute(gorn::AttributeKind::Position, "position");

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
		for(auto& cam : _gorn.getQueue().getCameras())
		{
			_root->touch(cam->getScreenToWorldPoint(p));
		}
    }

    void Context::touchEnd(const glm::vec2& p)
    {
		for (auto& cam : _gorn.getQueue().getCameras())
		{
			_root->touch(cam->getScreenToWorldPoint(p), EntityTouchPhase::End);
		}
    }

}
