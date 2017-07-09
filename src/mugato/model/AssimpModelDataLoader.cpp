

#include <mugato/model/AssimpModelDataLoader.hpp>
#include <mugato/scene/Entity.hpp>
#include <mugato/scene/MeshComponent.hpp>
#include <mugato/base/Exception.hpp>
#include <gorn/base/String.hpp>
#include <gorn/asset/Mesh.hpp>
#include <gorn/gl/MaterialDefinition.hpp>
#include <gorn/gl/MaterialManager.hpp>
#include <gorn/base/String.hpp>
#include <buffer.hpp>
#include <buffer_reader.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <iostream>

namespace mugato
{
    AssimpModelDataLoader::AssimpModelDataLoader(gorn::MaterialManager& materials) NOEXCEPT:
	_materials(materials)
	{
	}

	bool AssimpModelDataLoader::validate(const buffer& data) const NOEXCEPT
	{
		return true;
	}

	glm::mat4 AssimpModelDataLoaderLoadTransform(const aiMatrix4x4& from)
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

	gorn::MaterialDefinition AssimpModelDataLoader::loadMaterial(const aiMaterial* aimaterial) const
	{
        gorn::MaterialDefinition def;
		aiString aistr;
		aimaterial->Get(AI_MATKEY_NAME, aistr);
        def.withProgram(aistr.data);

		for (auto& type : _textureMap)
		{
            auto texCount = aimaterial->GetTextureCount(type.first);
			for (unsigned i = 0; i < texCount; i++)
			{
                aimaterial->GetTexture(type.first, i, &aistr);
                def.withTexture(gorn::UniformKind(type.second, i), aistr.data);
			}
		}
		return def;
	}

	gorn::Mesh AssimpModelDataLoader::loadMesh(const aiMesh* aimesh) const
	{
		gorn::Mesh mesh;
        mesh.getElements().reserve(aimesh->mNumFaces*4);
		for(unsigned i=0; i<aimesh->mNumFaces; i++)
		{
			auto& face = aimesh->mFaces[i];
            std::vector<gorn::Mesh::Element> elms;
            elms.resize(face.mNumIndices);
            for(unsigned int i = 0; i < face.mNumIndices; i++)
            {
                elms[i].setDefault(face.mIndices[i]);
            }
            mesh.addFace(elms);
		}

        mesh.reserveVertices<glm::vec3>(gorn::AttributeType::Position, aimesh->mNumVertices);
        for(unsigned i = 0; i < aimesh->mNumVertices; i++)
        {
            auto& pos = aimesh->mVertices[i];
            mesh.addVertex(gorn::AttributeType::Position, glm::vec3(
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
            mesh.reserveVertices<glm::vec2>(gorn::AttributeKind(gorn::AttributeType::TexCoords, j),
                aimesh->mNumVertices);
            for(unsigned i = 0; i < aimesh->mNumVertices; i++)
            {
                auto& uv = tex[i];
                mesh.addVertex(gorn::AttributeKind(gorn::AttributeType::TexCoords, j),
                    glm::vec2(uv.x, uv.y));
            }
        }
		if (aimesh->HasNormals())
		{
            mesh.reserveVertices<glm::vec3>(gorn::AttributeType::Normal, aimesh->mNumVertices);
            for(unsigned i = 0; i < aimesh->mNumVertices; i++)
            {
                auto& n = aimesh->mNormals[i];
                mesh.addVertex(gorn::AttributeType::Normal, glm::vec3(
                    n.x, n.y, n.z
                ));
            }
		}

		return mesh;
	}

	ModelData AssimpModelDataLoader::loadNode(const aiNode* ainode, const std::vector<ModelDataMesh>& meshes) const
	{
        ModelData node;
		node.setTransform(AssimpModelDataLoaderLoadTransform(ainode->mTransformation));
		for(unsigned i = 0; i < ainode->mNumMeshes; i++)
		{
			node.addMesh(meshes.at(ainode->mMeshes[i]));
		}
		for(unsigned i = 0; i < ainode->mNumChildren; i++)
		{
			node.addChild(loadNode(ainode->mChildren[i], meshes));
		}
		return node;
	}

	ModelData AssimpModelDataLoader::load(const buffer& data) const
	{
		Assimp::Importer importer;
		auto scene = importer.ReadFileFromMemory(data.data(), data.size(), 0);
		if(!scene)
		{
			throw Exception(importer.GetErrorString());
		}
		
        ModelData model;
		std::vector<std::string> matDefNames;
		matDefNames.reserve(scene->mNumMaterials);
		for (unsigned i = 0; i < scene->mNumMaterials; i++)
		{
			auto matDef = loadMaterial(scene->mMaterials[i]);
			auto matDefName = matDef.getProgram();
			_materials.getDefinitions().set(matDefName, matDef);
			matDefNames.push_back(matDefName);
		}
		std::vector<ModelDataMesh> meshes;
		meshes.reserve(scene->mNumMeshes);
		for (unsigned i = 0; i < scene->mNumMeshes; i++)
		{
			auto mesh = scene->mMeshes[i];			
			meshes.push_back(ModelDataMesh{
				std::make_shared<gorn::Mesh>(loadMesh(mesh)),
				matDefNames[mesh->mMaterialIndex]
			});
		}
		return loadNode(scene->mRootNode, meshes);
	}
}
