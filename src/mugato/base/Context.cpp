
#include <mugato/base/Context.hpp>
#include <mugato/base/Kinds.hpp>
#include <mugato/sprite/MaterialSpriteAtlasLoader.hpp>
#include <mugato/sprite/CocosSpriteAtlasLoader.hpp>
#include <mugato/sprite/GdxSpriteAtlasLoader.hpp>
#include <mugato/label/FntFontAtlasLoader.hpp>
#include <mugato/label/DebugFontAtlasConfigurator.hpp>
#include <gorn/render/ProgramManager.hpp>
#include <gorn/render/Kinds.hpp>
#include <gorn/asset/FileManager.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace mugato
{

    Context::Context():
    _sprites(_gorn.getMaterials(), _gorn.getFiles()),
    _labels(_gorn.getMaterials(), _gorn.getFiles()),
    _skeletons(_gorn.getMaterials(), _gorn.getFiles()),
    _screenSize(2.0f), _fixedUpdateInterval(0.0),
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
                    .withTexture(name)
                    .withProgram(ProgramKind::Sprite);
            });

	    _gorn.getPrograms().getDefinitions().get(ProgramKind::Sprite)
            .withShaderData(gorn::ShaderType::Vertex, R"(#version 100

precision highp float;

attribute vec3 position;
attribute vec2 texCoords;

uniform mat4 model;
uniform mat4 view;

varying vec2 TexCoords;

void main()
{
    TexCoords = texCoords;
    gl_Position = view * model * vec4(position, 1.0);
}

)")
            .withShaderData(gorn::ShaderType::Fragment, R"(#version 100

precision highp float;

varying vec2 TexCoords;

uniform sampler2D texture;

void main()
{
    gl_FragColor = texture2D(texture, TexCoords);
}

)")
            .withUniform(gorn::UniformKind::Model, "model")
            .withUniform(gorn::UniformKind::View, "view")
            .withUniform(gorn::UniformKind::Texture0, "texture")
            .withAttribute(gorn::AttributeKind::Position, "position")
            .withAttribute(gorn::AttributeKind::TexCoords, "texCoords");


 _gorn.getPrograms().getDefinitions().get(ProgramKind::Color)
            .withShaderData(gorn::ShaderType::Vertex, R"(#version 100

precision highp float;

attribute vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform vec4 color;

void main()
{
    gl_Position = view * model * vec4(position, 1.0);
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
            .withUniform(gorn::UniformKind::View, "view")
            .withUniform(gorn::UniformKind::Color, "color")
            .withAttribute(gorn::AttributeKind::Position, "position");

        DebugFontAtlasConfigurator().setup(*this);

        _scenes.setContext(*this);
        _scenes.getTransform().setSize(_screenSize);
    }

    void Context::setScreenSize(const glm::vec2& size)
    {
        _screenSize = size;
        _scenes.getTransform().setSize(size);
        auto trans = glm::translate(glm::mat4(), glm::vec3(-1.0f, -1.0f, 0.0f))
            *glm::scale(glm::mat4(), glm::vec3(2.0f/size.x, 2.0f/size.y, 1.0f));
        _gorn.getQueue().setViewTransform(trans);
    }

    const glm::vec2& Context::getScreenSize()
    {
        return _screenSize;
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
        return _scenes;
    }

    EntityStack& Context::getScenes()
    {
        return _scenes;
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
        _scenes.update(dt);
        _gorn.getQueue().update(dt);
    }

    void Context::fixedUpdate(double dt)
    {
        _scenes.fixedUpdate(dt);
    }

    void Context::draw()
    {
        _scenes.render(_gorn.getQueue());
        _gorn.getQueue().draw();
    }

}

