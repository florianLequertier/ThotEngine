#include "thotEngine/MeshRenderer.hpp"
#include "thotEngine/World.hpp"

namespace te{

MeshRenderer::MeshRenderer()
{
//    m_resourceManager = resourceManager;
//    m_materialManager = materialManager;
}

MeshRenderer::~MeshRenderer()
{

}

//init meshPtr and materialPtr
void MeshRenderer::pushToGPU()
{
    ResourceManager::getInstance().pushMeshToGPU(m_meshName);

    MaterialManager::getInstance().pushMaterialToGPU(m_materialName);
}

void MeshRenderer::popFromGPU()
{
    ResourceManager::getInstance().popMeshFromGPU(m_meshName);

    MaterialManager::getInstance().popMaterialFromGPU(m_materialName);
}

GLuint MeshRenderer::getMaterialID() const
{
    return m_materialPtr.lock()->getGlId();
}

void MeshRenderer::setMesh(std::string mesh)
{
    m_meshName = mesh;
    m_meshPtr = ResourceManager::getInstance().getMesh(m_meshName);
}

void MeshRenderer::setMaterial(std::string material)
{
    m_materialName = material;
    m_materialPtr = MaterialManager::getInstance().getMaterial(m_materialName);
}

void MeshRenderer::draw() const
{
    m_meshPtr.lock()->draw();
}

void MeshRenderer::render(const glm::mat4& worldMat, const glm::mat4& viewMat)
{
    //uniform matrix
    glm::mat4 modelMat = m_owner->getComponent<Transform>()->getModelMatrix();

    //set uniforms of the current material
    m_materialPtr.lock()->setUniforms(modelMat, worldMat, viewMat);

    //call opengl draw call
    draw();
}

void MeshRenderer::render(const glm::mat4& worldMat, const glm::mat4& viewMat, std::shared_ptr<CArray<PointLight>> pointLights, std::shared_ptr<CArray<DirectionalLight>> directionalLights, const glm::vec3& viewPosition)
{
    //uniform matrix
    glm::mat4 modelMat = m_owner->getComponent<Transform>()->getModelMatrix();

    //set uniforms of the current material
    m_materialPtr.lock()->setUniforms(modelMat, worldMat, viewMat, pointLights, directionalLights, viewPosition);

    //call opengl draw call
    draw();
}

}
