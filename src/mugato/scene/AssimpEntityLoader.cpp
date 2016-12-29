

#include <mugato/scene/AssimpEntityLoader.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/scene/MeshComponent.hpp>
#include <mugato/base/Exception.hpp>
#include <gorn/asset/Mesh.hpp>
#include <gorn/gl/MaterialManager.hpp>
#include <gorn/gl/TextureManager.hpp>
#include <gorn/gl/ProgramManager.hpp>
#include <gorn/base/String.hpp>
#include <buffer.hpp>
#include <buffer_reader.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>

namespace mugato
{
	AssimpEntityLoader::AssimpEntityLoader(gorn::MaterialManager& materials, const std::string& prefix) NOEXCEPT:
	_materials(materials), _prefix(prefix)
	{
	}

	bool AssimpEntityLoader::validate(const buffer& data) const NOEXCEPT
	{
		return true;
	}

	glm::mat4 AssimpEntityLoaderLoadMatrix(const aiMatrix4x4& from)
	{
		glm::mat4 to;
		to[0][0] = from.a1; to[1][0] = from.a2;
		to[2][0] = from.a3; to[3][0] = from.a4;
		to[0][1] = from.b1; to[1][1] = from.b2;
		to[2][1] = from.b3; to[3][1] = from.b4;
		to[0][2] = from.c1; to[1][2] = from.c2;
		to[2][2] = from.c3; to[3][2] = from.c4;
		to[0][3] = from.d1; to[1][3] = from.d2;
		to[2][3] = from.d3; to[3][3] = from.d4;
		return to;
	}

	const std::map<aiTextureType, gorn::UniformType> _textureMap = std::map<aiTextureType, gorn::UniformType>{
		{ aiTextureType_DIFFUSE, gorn::UniformType::DiffuseTexture },
		{ aiTextureType_SPECULAR, gorn::UniformType::SpecularTexture },
		{ aiTextureType_AMBIENT, gorn::UniformType::AmbientTexture },
		{ aiTextureType_EMISSIVE, gorn::UniformType::EmissiveTexture },
		{ aiTextureType_HEIGHT, gorn::UniformType::HeightTexture },
		{ aiTextureType_NORMALS, gorn::UniformType::NormalTexture },
		{ aiTextureType_SHININESS, gorn::UniformType::ShininessTexture },
		{ aiTextureType_OPACITY, gorn::UniformType::OpacityTexture },
		{ aiTextureType_DISPLACEMENT, gorn::UniformType::DisplacementTexture },
		{ aiTextureType_LIGHTMAP, gorn::UniformType::LightmapTexture },
		{ aiTextureType_REFLECTION, gorn::UniformType::ReflectionTexture },
	};

	std::shared_ptr<gorn::Material> AssimpEntityLoader::loadMaterial(const aiMaterial* aimaterial) const
	{
		aiString aistr;
		aimaterial->Get(AI_MATKEY_NAME, aistr);
        std::string materialName(aistr.data);
        gorn::String::addTag(materialName, _prefix);
        if(_materials.validate(materialName))
        {
            return _materials.load(materialName);
        }

        auto programName = materialName;
        gorn::String::addTag(programName, _prefix);
        auto program = _materials.getPrograms().load(programName);
		auto material = std::make_shared<gorn::Material>(program);

		for (auto& type : _textureMap)
		{
            auto texCount = aimaterial->GetTextureCount(type.first);
			for (unsigned i = 0; i < texCount; i++)
			{
                aimaterial->GetTexture(type.first, i, &aistr);
                std::string textureName(aistr.data);
                gorn::String::addTag(textureName, _prefix);
				auto texture = _materials.getTextures().load(textureName);
				material->setTexture(
					gorn::UniformKind(type.second, i), texture);
			}
		}
        _materials.preload(materialName, material);
		return material;
	}

	std::shared_ptr<gorn::Mesh> AssimpEntityLoader::loadMesh(const aiMesh* aimesh) const
	{
		auto mesh = std::make_shared<gorn::Mesh>();
        mesh->reserveElements(aimesh->mNumFaces);
		for(unsigned i=0; i<aimesh->mNumFaces; i++)
		{
			auto& face = aimesh->mFaces[i];
            for(int i = 0; i < face.mNumIndices; i++)
            {
                auto elm = gorn::MeshElement();
                elm.setDefault(face.mIndices[i]);
                mesh->addElement(std::move(elm));
            }
		}

        mesh->reserveVertices<glm::vec3>(gorn::AttributeType::Position, aimesh->mNumVertices);
        for(unsigned i = 0; i < aimesh->mNumVertices; i++)
        {
            auto& pos = aimesh->mVertices[i];
            mesh->addVertex(gorn::AttributeType::Position, glm::vec3(
                pos.x, pos.y, pos.z
            ));
        }
        for(unsigned j = 0; j < AI_MAX_NUMBER_OF_TEXTURECOORDS; j++)
        {
            auto tex = aimesh->mTextureCoords[j];
            if(tex == nullptr)
            {
                break;
            }
            mesh->reserveVertices<glm::vec2>(gorn::AttributeKind(gorn::AttributeType::TexCoords, j),
                aimesh->mNumVertices);
            for(unsigned i = 0; i < aimesh->mNumVertices; i++)
            {
                auto& uv = tex[i];
                mesh->addVertex(gorn::AttributeKind(gorn::AttributeType::TexCoords, j),
                    glm::vec2(uv.x, uv.y));
            }
        }
		if (aimesh->HasNormals())
		{
            mesh->reserveVertices<glm::vec3>(gorn::AttributeType::Normal, aimesh->mNumVertices);
            for(unsigned i = 0; i < aimesh->mNumVertices; i++)
            {
                auto& n = aimesh->mNormals[i];
                mesh->addVertex(gorn::AttributeType::Normal, glm::vec3(
                    n.x, n.y, n.z
                ));
            }
		}

		return mesh;
	}

	std::shared_ptr<Entity> AssimpEntityLoader::loadNode(const aiNode* node, const AssimpContext& ctx) const
	{
		auto e = std::make_shared<Entity>();
		auto m = AssimpEntityLoaderLoadMatrix(node->mTransformation);
		e->getTransform().setMatrix(m);

		for(unsigned i = 0; i < node->mNumMeshes; i++)
		{
			auto meshIdx = node->mMeshes[i];
			auto materialIdx = ctx.scene->mMeshes[meshIdx]->mMaterialIndex;
			e->addComponent<MeshComponent>(
				ctx.meshes[meshIdx],
				ctx.materials[materialIdx]
			);
		}

		for(unsigned i = 0; i < node->mNumChildren; i++)
		{
			auto c = loadNode(node->mChildren[i], ctx);
			e->addChild(c);
		}
		return e;
	}

	std::shared_ptr<Entity> AssimpEntityLoader::load(const buffer& data) const
	{
		Assimp::Importer importer;
		auto scene = importer.ReadFileFromMemory(data.data(), data.size(), 0);
		if(!scene)
		{
			throw Exception(importer.GetErrorString());
		}

		auto ctx = AssimpContext();
		ctx.scene = scene;
		for (unsigned i = 0; i < scene->mNumMaterials; i++)
		{
			auto m = scene->mMaterials[i];
			ctx.materials.push_back(loadMaterial(m));
		}
		for (unsigned i = 0; i < scene->mNumMeshes; i++)
		{
			auto aimesh = scene->mMeshes[i];
			ctx.meshes.push_back(loadMesh(aimesh));
		}
		return loadNode(scene->mRootNode, ctx);
	}
}
