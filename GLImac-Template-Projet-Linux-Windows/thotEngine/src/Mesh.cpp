#include <stdlib.h>
#include <stdio.h>
#include <memory>

#include <SDL\SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>

#include "thotEngine/GLEntity.hpp"
#include "thotEngine/Material.hpp"
#include "thotEngine/ResourceManager.hpp"
#include "thotEngine/Mesh.hpp"

namespace te
{

Mesh::Mesh(std::string name) : m_name(name)
{

}

Mesh::~Mesh()
{

}

bool Mesh::load(const std::string& path)
{
    clear();

    bool Ret = false;
    Assimp::Importer Importer;

    const aiScene* pScene = Importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

    if (pScene) {
        Ret = initFromScene(applicationPath, pScene, path);
    }
    else {
        std::cout << "Error parsing " << path << " : " << Importer.GetErrorString() << std::endl;
    }

    return Ret;
}

void Mesh::pushToGPU()
{
    for (int i = 0; i < m_entities.size(); ++i)
    {
        m_entities[i].pushToGPU();
    }
}

void Mesh::popFromGPU()
{
    for (int i = 0; i < m_entities.size(); ++i)
    {
        m_entities[i].popFromGPU();
    }
}

//std::shared_ptr<Material> Mesh::getMaterial() const
//{
//	return m_material;
//}
//
//void Mesh::setMaterial(std::shared_ptr<Material> material)
//{
//	m_material = material;
//}

void Mesh::draw()
{
    for (int i = 0; i < m_entities.size(); ++i)
    {
        m_entities[i].draw();
    }
}

bool Mesh::initFromScene(const aiScene* pScene, const std::string& Filename/*, bool loadTextures*/)
{
    m_entities.resize(pScene->mNumMeshes);

    // Initialize the meshes in the scene one by one
    for (unsigned int i = 0; i < m_entities.size(); i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        initMesh(i, paiMesh);
    }

    //if (loadTextures)
    //	return initMaterial(engine, pScene, Filename);
    //else
    return true;
}

void Mesh::initMesh(unsigned int Index, const aiMesh* paiMesh)
{
    m_entities[Index].setMaterialIndex(paiMesh->mMaterialIndex);

    std::vector<gl::Vertex > Vertices;
    std::vector<GLuint> Indices;

    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
        const aiVector3D* pPos = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal = paiMesh->HasNormals() ? &(paiMesh->mNormals[i]) : &Zero3D;
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        gl::Vertex v(glm::vec3(pPos->x, pPos->y, pPos->z),
            glm::vec3(pNormal->x, pNormal->y, pNormal->z),
            glm::vec2(pTexCoord->x, pTexCoord->y));

        Vertices.push_back(v);
    }

    for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }

    m_entities[Index].init(Vertices, Indices);
}

void Mesh::init(const std::vector<gl::Vertex>& vertices, const std::vector<uint32_t>& indices/*, std::shared_ptr<Material> material*/)
{
    m_entities.clear();

    GLEntity newEntity;
    newEntity.init(vertices, indices);

    m_entities.push_back(newEntity);

    //m_material = material;
}

void Mesh::clear()
{
    //if (m_material != nullptr)
    //	m_material.reset();

    m_entities.clear();
}


//
//bool Mesh::initMaterial(Engine& engine, const aiScene* pScene, const std::string& path)
//{
//	m_material = engine.createMaterial(Material::MaterialType::UNLIT_TEX_3D);
//
//	for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
//	{
//		const aiMaterial* pMaterial = pScene->mMaterials[i];
//
//		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
//		{
//			aiString texPath;
//
//			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texPath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
//			{
//				auto newTexture = engine.loadTexture(std::string("MeshModel_").append(m_name).append("_tex_diffuse"), texPath.C_Str());
//				if (newTexture != nullptr)
//				{
//					newTexture->pushToGPU();
//					m_material->addTexture(newTexture);
//				}
//				else
//					return false;
//			}
//		}
//
//	}
//	return true;
//}
//
//
//bool Mesh::initMaterial(const std::string& applicationPath, const aiScene* pScene, const std::string& path)
//{
//	m_material = std::shared_ptr<Material>(new Material(Material::MaterialType::UNLIT_TEX_3D, applicationPath));
//
//	for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
//	{
//		const aiMaterial* pMaterial = pScene->mMaterials[i];
//
//		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
//		{
//			aiString texPath;
//
//			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texPath, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
//			{
//				auto newTexture = gl::loadImage(texPath.C_Str());// engine.loadTexture(std::string("MeshModel_").append(m_name).append("_tex_diffuse"), path);
//				if (newTexture != nullptr)
//				{
//					newTexture->pushToGPU();
//					m_material->addTexture(newTexture);
//				}
//				else
//					return false;
//			}
//		}
//
//	}
//	return true;
//}

}
