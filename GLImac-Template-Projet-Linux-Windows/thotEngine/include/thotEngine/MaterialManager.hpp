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

    //internals
    std::map<std::string, std::shared_ptr<Material> > m_internalMaterials; // name <-> material
    std::map<std::string, int> m_internalMaterialCount; // name <-> count

public:

    //singleton
    static MaterialManager& getInstance();
    ~MaterialManager();
    //void init(std::shared_ptr<ResourceManager> resourceManager);

    void loadInternals();

    void pushMaterialToGPU(std::string name, ResourceAccessType accessType = ResourceAccessType::EXTERNAL);
    void popMaterialFromGPU(std::string name, ResourceAccessType accessType = ResourceAccessType::EXTERNAL);

    template<typename MaterialType>
    std::shared_ptr<Material> createMaterial(std::string name, std::string programName, ResourceAccessType accessType = ResourceAccessType::EXTERNAL);

    //std::shared_ptr<Material> createMaterial<SkyboxMaterial>(std::string name, std::string programName);

    template<typename MaterialType>
    std::shared_ptr<Material> createMaterial(std::string name, std::string programName, std::vector<std::string> imgNames, ResourceAccessType accessType = ResourceAccessType::EXTERNAL);

    //std::shared_ptr<Material> createMaterial<SkyboxMaterial>(std::string name, std::string programName, std::vector<std::string> imgNames);

    template<typename MaterialType>
    std::shared_ptr<Material> createMaterial(std::string name, std::string programName, std::vector<std::string> imgNames, std::vector<float> parameters, ResourceAccessType accessType = ResourceAccessType::EXTERNAL);

    //std::shared_ptr<Material> createMaterial<SkyboxMaterial>(std::string name, std::string programName, std::vector<std::string> imgNames, std::vector<float> parameters);

    std::shared_ptr<Material> getMaterial(std::string name, ResourceAccessType accessType = ResourceAccessType::EXTERNAL);

    bool containsMaterial(std::string name, ResourceAccessType accessType = ResourceAccessType::EXTERNAL);

private:
    MaterialManager();
    MaterialManager(const MaterialManager& other);
    MaterialManager(const MaterialManager&& other);
    MaterialManager& operator=(const MaterialManager& other);

};





template<typename MaterialType>
inline std::shared_ptr<Material> MaterialManager::createMaterial(std::string name, std::string programName, ResourceAccessType accessType)
{
    auto program = ResourceManager::getInstance().getProgram(programName);

    if (!program)
    {
        std::cout << "error when creating material with name : " << programName << " there is no program with this name" << std::endl;
        return nullptr;
    }

    //make a new material instance, with name [parameter : name], and with a program which name is programName
    auto newMat = std::make_shared<MaterialType>(program); //std::shared_ptr<Material>(new Material( program ));

    if(accessType == ResourceAccessType::EXTERNAL)
    {
        //store it in the manager
        m_materials[name] = newMat;
        m_materialCount[name] = 0;
    }
    else
    {
        //store it in the manager
        m_internalMaterials[name] = newMat;
        m_internalMaterialCount[name] = 0;
    }

    return newMat;
}

template<typename MaterialType>
inline std::shared_ptr<Material> MaterialManager::createMaterial(std::string name, std::string programName, std::vector<std::string> imgNames, ResourceAccessType accessType)
{
    auto program = ResourceManager::getInstance().getProgram(programName);
    std::vector<std::shared_ptr<Image>> images;
    for(auto imgName : imgNames)
    {
        images.push_back(ResourceManager::getInstance().getImage(imgName));
    }

    if (!program)
    {
        std::cout << "error when creating material with name : " << programName << " there is no program with this name" << std::endl;
        return nullptr;
    }

    //make a new material instance, with name [parameter : name], and with a program which name is programName
    auto newMat = std::make_shared<MaterialType>(program, images); //std::shared_ptr<Material>(new Material( program, images ));

    if(accessType == ResourceAccessType::EXTERNAL)
    {
        //store it in the manager
        m_materials[name] = newMat;
        m_materialCount[name] = 0;
    }
    else
    {
        //store it in the manager
        m_internalMaterials[name] = newMat;
        m_internalMaterialCount[name] = 0;
    }

    std::cout<<"creating new material : "<<name<<" successfully !"<<std::endl;
    return newMat;
}

template<typename MaterialType>
inline std::shared_ptr<Material> MaterialManager::createMaterial(std::string name, std::string programName, std::vector<std::string> imgNames, std::vector<float> parameters, ResourceAccessType accessType)
{
    auto program = ResourceManager::getInstance().getProgram(programName);
    std::vector<std::shared_ptr<Image>> images;
    for(auto imgName : imgNames)
    {
        images.push_back(ResourceManager::getInstance().getImage(imgName));
    }

    if (!program)
    {
        std::cout << "error when creating material with name : " << programName << " there is no program with this name" << std::endl;
        return nullptr;
    }

    //make a new material instance, with name [parameter : name], and with a program which name is programName
    auto newMat = std::make_shared<MaterialType>(program, images, parameters); //std::shared_ptr<Material>(new Material( program, images ));

    if(accessType == ResourceAccessType::EXTERNAL)
    {
        //store it in the manager
        m_materials[name] = newMat;
        m_materialCount[name] = 0;
    }
    else
    {
        //store it in the manager
        m_internalMaterials[name] = newMat;
        m_internalMaterialCount[name] = 0;
    }

    std::cout<<"creating new material : "<<name<<" successfully !"<<std::endl;
    return newMat;
}

template<>
std::shared_ptr<Material> MaterialManager::createMaterial<SkyboxMaterial>(std::string name, std::string programName, ResourceAccessType accessType);
template<>
std::shared_ptr<Material> MaterialManager::createMaterial<SkyboxMaterial>(std::string name, std::string programName, std::vector<std::string> imgNames, ResourceAccessType accessType);
template<>
std::shared_ptr<Material> MaterialManager::createMaterial<SkyboxMaterial>(std::string name, std::string programName, std::vector<std::string> imgNames, std::vector<float> parameters, ResourceAccessType accessType);



}


#endif // MATERIALMANAGER_HPP
