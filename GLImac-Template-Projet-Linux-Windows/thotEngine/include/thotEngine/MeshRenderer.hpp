#ifndef MESHRENDERER_HPP
#define MESHRENDERER_HPP

#include "thotEngine/Mesh.hpp"
#include "thotEngine/Material.hpp"
#include "thotEngine/ResourceManager.hpp"
#include "thotEngine/MaterialManager.hpp"
#include "thotEngine/Component.hpp"

#include "thotEngine/light.hpp"

namespace te {

class MeshRenderer : public Component, public BaseWorldObject<MeshRenderer>
{
private:
//    std::weak_ptr<ResourceManager> m_resourceManager;
//    std::weak_ptr<MaterialManager> m_materialManager;

    std::string m_meshName;
    std::string m_materialName;

    std::weak_ptr<Mesh> m_meshPtr;
    std::weak_ptr<Material> m_materialPtr;


public:
MeshRenderer();
~MeshRenderer();

virtual void init() override;

void pushToGPU(); //init meshPtr and materialPtr
void popFromGPU();

void setMesh(std::string mesh);
void setMaterial(std::string material);

GLuint getMaterialID() const;

void draw() const;
void render(const glm::mat4& worldMat, const glm::mat4& viewMat);
void render(const glm::mat4& worldMat, const glm::mat4& viewMat, std::shared_ptr<CArray<PointLight>> pointLights, std::shared_ptr<CArray<DirectionalLight>> directionalLights, const glm::vec3& viewPosition);

//operator overload
inline bool operator<(const MeshRenderer& other )
{
    return getMaterialID() < other.getMaterialID();
}

};

}

#endif // MESHRENDERER_HPP
