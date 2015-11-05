#ifndef MATERIALMANAGER_HPP
#define MATERIALMANAGER_HPP

#include <memory>
#include "ResourceManager.hpp"
#include "Material.hpp"

namespace te{

class MaterialManager
{

private:

    std::weak_ptr<ResourceManager> m_resourceManager;

    //material instances
    std::map<std::string, std::shared_ptr<Material> > m_materials; // name <-> material
    std::map<std::string, int> m_materialCount; // name <-> count

public:
    MaterialManager();
    ~MaterialManager();
    void init(std::shared_ptr<ResourceManager> resourceManager);

    void pushMaterialToGPU(std::string name);
    void popMaterialFromGPU(std::string name);

    std::shared_ptr<Material> createMaterial(std::string name, std::string programName);
    std::shared_ptr<Material> createMaterial(std::string name, std::string programName, std::vector<std::shared_ptr<Image> > textures);

};

}

#endif // MATERIALMANAGER_HPP
