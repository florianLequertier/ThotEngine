
#include "thotEngine/MaterialManager.hpp"

namespace te{

//singleton (c++11 implementation)
MaterialManager& MaterialManager::getInstance()
{
    static MaterialManager instance;
    return instance;
}

//privates
MaterialManager::MaterialManager(){}
MaterialManager::MaterialManager(const MaterialManager& other){}
MaterialManager::MaterialManager(const MaterialManager&& other){}
MaterialManager& MaterialManager::operator=(const MaterialManager& other){return *this;}

MaterialManager::~MaterialManager()
{

}

//void MaterialManager::init(std::shared_ptr<ResourceManager> resourceManager)
//{
//    m_resourceManager = resourceManager;
//}


void MaterialManager::pushMaterialToGPU(std::string name)
{
    //try to found material name among meshes which have already been loaded
    assert( m_materials.find(name)!= m_materials.end()  );

    //if material isn't present in scene (hasn't already be pushed to gpu)
    if(m_materialCount[name] == 0)
    {
        //push the material to the gpu
       m_materials[name]->pushToGPU();
    }
    //update material use counter
    m_materialCount[name]++;
}

void MaterialManager::popMaterialFromGPU(std::string name)
{
    //try to found material name among meshes which have already been loaded
    assert( m_materials.find(name)!= m_materials.end() );

    //update material use counter
    m_materialCount[name]--;
    //if material has totaly disappeared from the sceen
    if(m_materialCount[name] == 0)
    {
       //pop the material to the gpu
       m_materials[name]->popFromGPU();
    }
}

std::shared_ptr<Material> MaterialManager::getMaterial(std::string name)
{
    if( m_materials.find(name)!= m_materials.end() )
    {
        return m_materials[name];
    }
    else
    {
        return std::shared_ptr<Material>();
    }

}

bool MaterialManager::containsMaterial(std::string name)
{
    return m_materials.find(name)!= m_materials.end();
}

//std::shared_ptr<Material> MaterialManager::createMaterial(std::string name, std::string programName)
//{
//    if (m_programs.find(programName) == m_programs.end())
//    {
//        std::cout << "error when creating material with name : " << programName << " there is no program with this name" << std::endl;
//        return nullptr;
//    }

//    //make a new material instance, with name [parameter : name], and with a program which name is programName
//    auto newMat = std::shared_ptr<Material>(new Material(m_programs[programName]));

//    //store it in the manager
//    m_materials[name] = newMat;
//    m_materialCount[name] = 0;

//    return newMat;
//}

//std::shared_ptr<Material> MaterialManager::createMaterial(std::string name, std::string programName, std::vector<std::shared_ptr<Image> > textures)
//{
//    if (m_programs.find(programName) == m_programs.end())
//    {
//        std::cout << "error when creating material with name : " << programName << " there is no program with this name" << std::endl;
//        return nullptr;
//    }

//    //make a new material instance, with name [parameter : name], and with a program which name is programName
//    auto newMat = std::shared_ptr<Material>(new Material(m_programs[programName], textures));

//    //store it in the manager
//    m_materials[name] = newMat;
//    m_materialCount[name] = 0;

//    return newMat;
//}

}
