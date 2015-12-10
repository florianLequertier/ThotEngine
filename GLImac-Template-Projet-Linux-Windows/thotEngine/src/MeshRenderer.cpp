#include "thotEngine/MeshRenderer.hpp"
#include "thotEngine/World.hpp"

//forward
#include "thotEngine/World.hpp"

namespace te{

MeshRenderer::MeshRenderer()
{
//    m_resourceManager = resourceManager;
//    m_materialManager = materialManager;
}

MeshRenderer::~MeshRenderer()
{
    popFromGPU();
}

void MeshRenderer::init(World &world)
{
    //nothing
}

//init meshPtr and materialPtr
void MeshRenderer::pushToGPU()
{
    ResourceManager::getInstance().pushMeshToGPU(m_meshName, m_meshAccessType);

    MaterialManager::getInstance().pushMaterialToGPU(m_materialName, m_materialAccessType);
}

void MeshRenderer::popFromGPU()
{
    ResourceManager::getInstance().popMeshFromGPU(m_meshName, m_meshAccessType);

    MaterialManager::getInstance().popMaterialFromGPU(m_materialName, m_materialAccessType);
}

GLuint MeshRenderer::getMaterialID() const
{
    return m_materialPtr.lock()->getGlId();
}

void MeshRenderer::setMesh(std::string mesh, ResourceAccessType accessType)
{
    m_meshName = mesh;
    m_meshAccessType = accessType;
    m_meshPtr = ResourceManager::getInstance().getMesh(m_meshName);

    //push graphics :
    ResourceManager::getInstance().pushMeshToGPU(m_meshName, m_meshAccessType);
}

void MeshRenderer::setMaterial(std::string material, ResourceAccessType accessType)
{
    m_materialName = material;
    m_materialAccessType = accessType;
    m_materialPtr = MaterialManager::getInstance().getMaterial(m_materialName);

    //push graphics :
    MaterialManager::getInstance().pushMaterialToGPU(m_materialName, m_materialAccessType);
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


glm::vec3 MeshRenderer::getUpperRight() const
{
    if(m_meshPtr.expired())
        return glm::vec3(0,0,0);
    return m_meshPtr.lock()->getUpperRight();
}

glm::vec3 MeshRenderer::getLowerLeft() const
{
    if(m_meshPtr.expired())
        return glm::vec3(0,0,0);
    return m_meshPtr.lock()->getLowerLeft();
}

}

