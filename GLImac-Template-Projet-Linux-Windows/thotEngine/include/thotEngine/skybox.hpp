#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <string>
#include "Mesh.hpp"
#include "Material.hpp"

namespace te{

class Skybox
{

private:
    std::shared_ptr<Material> m_skyMaterial;
    bool m_isInitialized;
    GLEntity m_glEntity;
public:
    Skybox();
    ~Skybox();

    void pushToGPU();
    void popFromGPU();

    bool isInitialized() const;

    void setMaterial(std::string materialName);
    void render(const glm::mat4& modelMat, const glm::mat4& worldMat, const glm::mat4& viewMat);
};

}

#endif // SKYBOX_HPP

