#ifndef MATERIALMANAGER_HPP
#define MATERIALMANAGER_HPP

#include <memory>
#include "ResourceManager.hpp"
#include "Material.hpp"

namespace te{

class MaterialManager
{

private:

    //std::weak_ptr<ResourceManager> m_resourceManager;

    //material instances
    std::map<std::string, std::shared_ptr<Material> > m_materials; // name <-> material
    std::map<std::string, int> m_materialCount; // name <-> count

public:

    //singleton
    static MaterialManager& getInstance();
    ~MaterialManager();
    //void init(std::shared_ptr<ResourceManager> resourceManager);

    void pushMaterialToGPU(std::string name);
    void popMaterialFromGPU(std::string name);

    std::shared_ptr<Material> createMaterial(std::string name, std::string programName);
    std::shared_ptr<Material> createMaterial(std::string name, std::string programName, std::vector<std::string> imgNames);

    std::shared_ptr<Material> getMaterial(std::string name);

    bool containsMaterial(std::string name);

private:
    MaterialManager();
    MaterialManager(const MaterialManager& other);
    MaterialManager(const MaterialManager&& other);
    MaterialManager& operator=(const MaterialManager& other);

};

}

#endif // MATERIALMANAGER_HPP
