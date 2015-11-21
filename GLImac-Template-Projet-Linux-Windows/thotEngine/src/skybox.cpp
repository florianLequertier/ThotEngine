#include "thotEngine/glm.hpp"
#include <gl/glew.h>

#include "thotEngine/MaterialManager.hpp"
#include "thotEngine/skybox.hpp"

namespace te {

Skybox::Skybox()
{
    std::vector<Vertex> tab_data_vertexModel{

        //face x>0
        Vertex(glm::vec3(1, 1, -1), glm::vec3(1,0,0), glm::vec2(0.f, 2/3.f) ),
        Vertex(glm::vec3(1, 1, 1), glm::vec3(1,0,0),  glm::vec2(0, 1/3.f) ),
        Vertex(glm::vec3(1, -1, 1), glm::vec3(1,0,0), glm::vec2(1/4.f, 1/3.f) ),
        Vertex(glm::vec3(1, -1, -1), glm::vec3(1,0,0), glm::vec2(1/4.f, 2/3.f) ),

        //face x<0
        Vertex(glm::vec3(-1, 1, -1), glm::vec3(-1,0,0),glm::vec2(3/4.f, 2/3.f) ),
        Vertex(glm::vec3(-1, 1, 1), glm::vec3(-1,0,0), glm::vec2(3/4.f, 1/3.f)),
        Vertex(glm::vec3(-1, -1, 1), glm::vec3(-1,0,0), glm::vec2(2/4.f, 1/3.f)),
        Vertex(glm::vec3(-1, -1, -1), glm::vec3(-1,0,0), glm::vec2(2/4.f, 2/3.f)),

        //face z>0
        Vertex(glm::vec3(-1, 1, 1), glm::vec3(0,0,1), glm::vec2(2/4.f, 0.f) ),
        Vertex(glm::vec3(1, 1, 1), glm::vec3(0,0,1),  glm::vec2(1/4.f, 0.f) ),
        Vertex(glm::vec3(1, -1, 1), glm::vec3(0,0,1), glm::vec2(1/4.f, 1/3.f) ),
        Vertex(glm::vec3(-1, -1, 1), glm::vec3(0,0,1), glm::vec2(2/4.f, 1/3.f) ),

        //face z<0
        Vertex(glm::vec3(-1, 1, -1), glm::vec3(0,0,-1), glm::vec2(1/4.f, 2/3.f) ),
        Vertex(glm::vec3(1, 1, -1), glm::vec3(0,0,-1),  glm::vec2(2/4.f, 2/3.f) ),
        Vertex(glm::vec3(1, -1, -1), glm::vec3(0,0,-1), glm::vec2(1/4.f, 1.f) ),
        Vertex(glm::vec3(-1, -1, -1), glm::vec3(0,0,-1), glm::vec2(2/4.f, 1.f) ),

        //face y>0
        Vertex(glm::vec3(1, 1, 1), glm::vec3(0,1,0), glm::vec2(1.f, 1/3.f)),
        Vertex(glm::vec3(-1, 1, 1), glm::vec3(0,1,0), glm::vec2(3/4.f, 1/3.f) ),
        Vertex(glm::vec3(-1, 1, -1),glm::vec3(0,1,0),  glm::vec2(3/4.f, 2/3.f)),
        Vertex(glm::vec3(1, 1, -1),  glm::vec3(0,1,0), glm::vec2(1.f, 2/3.f)),

        //face y<0
        Vertex(glm::vec3(-1, -1, -1), glm::vec3(0,-1,0), glm::vec2(2/4.f, 2/3.f)),
        Vertex(glm::vec3(1, -1, -1), glm::vec3(0,-1,0), glm::vec2(1/4.f, 2/3.f)),
        Vertex(glm::vec3(1, -1, 1), glm::vec3(0,-1,0), glm::vec2(1/4.f, 1/3.f)),
        Vertex(glm::vec3(-1, -1, 1), glm::vec3(0,-1,0), glm::vec2(2/4.f, 1/3.f)),

    };

    std::vector<uint32_t> indices{
            0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 8, 9, 10, 10, 11, 8, 12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20,
    };

    m_glEntity.init(tab_data_vertexModel, indices);

    pushToGPU();
}

Skybox::~Skybox()
{

}

void Skybox::pushToGPU()
{
    m_glEntity.pushToGPU();
}

void Skybox::popFromGPU()
{
    m_glEntity.popFromGPU();
    m_skyMaterial->popFromGPU();
}

bool Skybox::isInitialized() const
{
    return m_isInitialized;
}

void Skybox::setMaterial(std::string materialName)
{
    //do nothing if material not found in MaterialManager
    if(!MaterialManager::getInstance().containsMaterial(materialName))
        return;

    //remove previous material if any
    if(m_skyMaterial)
    {
        m_skyMaterial->popFromGPU();
        m_skyMaterial.reset();
    }

    m_skyMaterial = MaterialManager::getInstance().getMaterial(materialName);

    if(m_skyMaterial)
    {
        m_skyMaterial->pushToGPU();
        m_isInitialized = true;
    }
}

void Skybox::render(const glm::mat4 &modelMat, const glm::mat4& worldMat, const glm::mat4& viewMat)
{
    glm::mat4 modelMatrix = modelMat;

    m_skyMaterial->use();
    m_skyMaterial->setUniforms(modelMatrix, worldMat, viewMat);

    m_glEntity.draw();

}

}
