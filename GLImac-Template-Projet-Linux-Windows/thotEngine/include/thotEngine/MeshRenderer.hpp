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
    ResourceAccessType m_meshAccessType;
    std::string m_materialName;
    ResourceAccessType m_materialAccessType;

    std::weak_ptr<Mesh> m_meshPtr;
    std::weak_ptr<Material> m_materialPtr;


public:
MeshRenderer();
~MeshRenderer();

virtual void init(World& world) override;

void pushToGPU(); //init meshPtr and materialPtr
void popFromGPU();

void setMesh(std::string mesh, ResourceAccessType accessType = ResourceAccessType::EXTERNAL);
void setMaterial(std::string material, ResourceAccessType accessType = ResourceAccessType::EXTERNAL);

GLuint getMaterialID() const;

void draw() const;
void render(const glm::mat4& worldMat, const glm::mat4& viewMat);
void render(const glm::mat4& worldMat, const glm::mat4& viewMat, std::shared_ptr<CArray<PointLight>> pointLights, std::shared_ptr<CArray<DirectionalLight>> directionalLights, const glm::vec3& viewPosition);

glm::vec3 getUpperRight() const;
glm::vec3 getLowerLeft() const;

//operator overload
inline bool operator<(const MeshRenderer& other )
{
    return getMaterialID() < other.getMaterialID();
}

};

}

#endif // MESHRENDERER_HPP
