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

    template<typename MaterialType>
    std::shared_ptr<Material> createMaterial(std::string name, std::string programName);
    template<typename MaterialType>
    std::shared_ptr<Material> createMaterial(std::string name, std::string programName, std::vector<std::string> imgNames);
    template<typename MaterialType>
    std::shared_ptr<Material> createMaterial(std::string name, std::string programName, std::vector<std::string> imgNames, std::vector<float> parameters);

    std::shared_ptr<Material> getMaterial(std::string name);

    bool containsMaterial(std::string name);

private:
    MaterialManager();
    MaterialManager(const MaterialManager& other);
    MaterialManager(const MaterialManager&& other);
    MaterialManager& operator=(const MaterialManager& other);

};

template<typename MaterialType>
std::shared_ptr<Material> MaterialManager::createMaterial(std::string name, std::string programName)
{
    auto program = ResourceManager::getInstance().getProgram(programName);

    if (!program)
    {
        std::cout << "error when creating material with name : " << programName << " there is no program with this name" << std::endl;
        return nullptr;
    }

    //make a new material instance, with name [parameter : name], and with a program which name is programName
    auto newMat = std::make_shared<MaterialType>(program); //std::shared_ptr<Material>(new Material( program ));

    //store it in the manager
    m_materials[name] = newMat;
    m_materialCount[name] = 0;

    return newMat;
}

template<typename MaterialType>
std::shared_ptr<Material> MaterialManager::createMaterial(std::string name, std::string programName, std::vector<std::string> imgNames)
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

    //store it in the manager
    m_materials[name] = newMat;
    m_materialCount[name] = 0;

    std::cout<<"creating new material : "<<name<<" successfully !"<<std::endl;
    return newMat;
}

template<typename MaterialType>
std::shared_ptr<Material> MaterialManager::createMaterial(std::string name, std::string programName, std::vector<std::string> imgNames, std::vector<float> parameters)
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

    //store it in the manager
    m_materials[name] = newMat;
    m_materialCount[name] = 0;

    std::cout<<"creating new material : "<<name<<" successfully !"<<std::endl;
    return newMat;
}

}

#endif // MATERIALMANAGER_HPP
