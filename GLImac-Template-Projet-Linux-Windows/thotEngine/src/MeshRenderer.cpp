#include "thotEngine/MeshRenderer.hpp"

namespace te{

MeshRenderer::MeshRenderer()
{

}

MeshRenderer::~MeshRenderer()
{

}

//init meshPtr and materialPtr
void MeshRenderer::pushToGPU()
{
    m_resourceManager.pushToGPU(m_meshName);
    m_meshGlId = m_resourceManager->getMeshId(m_meshName);

    m_resourceManager.pushToGPU(m_materialName);
    m_materialGlId = m_resourceManager->getMeshId(m_materialName);
}

void MeshRenderer::popFromGPU()
{
    m_resourceManager.popFromGPU(m_meshName);
    m_meshGlId = -1;

    m_resourceManager.popFromGPU(m_materialName);
    m_materialGlId = -1;
}

GLuint MeshRenderer::getMeshGlId() const
{
    return m_meshGlId;
}

GLuint MeshRenderer::getMaterialGlId() const
{
    return m_materialGlId;
}

void MeshRenderer::setMesh(std::string mesh)
{
    m_meshName = mesh;
}

void MeshRenderer::setMaterial(std::string material)
{
    m_materialName = material;
}

}
