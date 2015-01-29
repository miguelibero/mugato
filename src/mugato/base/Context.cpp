
#include <mugato/base/Context.hpp>
#include <mugato/base/Kinds.hpp>
#include <mugato/sprite/CocosSpriteAtlasLoader.hpp>
#include <mugato/sprite/GdxSpriteAtlasLoader.hpp>
#include <mugato/label/FntFontAtlasLoader.hpp>
#include <gorn/render/ProgramManager.hpp>
#include <gorn/render/Kinds.hpp>
#include <gorn/asset/FileManager.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace mugato
{

    Context::Context():
    _sprites(_gorn.getMaterials(), _gorn.getFiles()),
    _labels(_gorn.getMaterials(), _gorn.getFiles())
    {
        _sprites.getAtlases().addDefaultDataLoader<GdxSpriteAtlasLoader>();
        _sprites.getAtlases().addDefaultDataLoader<CocosSpriteAtlasLoader>();
        _labels.getAtlases().addDefaultDataLoader<FntFontAtlasLoader>();

        _gorn.getMaterials().getDefinitions().set(
            [](const std::string& name){
                return gorn::MaterialDefinition()
                    .withTexture(name)
                    .withProgram(ProgramKind::Sprite);
            });

	    _gorn.getPrograms().getDefinitions().get(ProgramKind::Sprite)
            .withShaderData(gorn::ShaderType::Vertex, R"(#version 100

precision highp float;

attribute vec2 position;
attribute vec2 texCoords;

varying vec2 TexCoords;

uniform mat4 transform;

void main()
{
    TexCoords = texCoords;
    gl_Position = transform*vec4(position, 0.0, 1.0);
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
            .withUniform("transform", gorn::UniformKind::Transform)
            .withUniform("texture", gorn::UniformKind::Texture0)
            .withAttribute("position", gorn::AttributeKind::Position)
            .withAttribute("texCoords", gorn::AttributeKind::TexCoords);
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
}

