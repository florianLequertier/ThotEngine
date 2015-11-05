#ifndef MESHRENDERER_HPP
#define MESHRENDERER_HPP

#include "thotEngine/Mesh.hpp"
#include "thotEngine/Material.hpp"
#include "thotEngine/ResourceManager.hpp"

namespace te {

class MeshRenderer
{
private:
    std::shared_ptr<ResourceManager> m_resourceManager;

    std::string m_meshName;
    std::string m_materialName;

    GLuint m_meshGlId;
    GLuint m_materialGlId;


public:
MeshRenderer(std::shared_ptr<ResourceManager> resourceManager);
~MeshRenderer();

void pushToGPU(); //init meshPtr and materialPtr
void popFromGPU();

GLuint getMeshGlId() const;
GLuint getMaterialGlId() const;
void setMesh(std::string mesh);
void setMaterial(std::string material);

};

}

#endif // MESHRENDERER_HPP
