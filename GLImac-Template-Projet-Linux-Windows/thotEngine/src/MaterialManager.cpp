
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

void MaterialManager::loadInternals()
{
    //unlit
    createMaterial<te::UnlitMaterial>("unlit_mat", "glProg_3D", {}, ResourceAccessType::INTERNAL);
    //lit
    createMaterial<te::LitMaterial>("lit_mat", "glProg_3DLight", {}, {0.4f,32.f}, ResourceAccessType::INTERNAL);
    //skybox
    createMaterial<te::UnlitMaterial>("skybox_mat", "glProg_skybox", {}, ResourceAccessType::INTERNAL);
    //shadowMapMaterial
    createMaterial<te::LightPassMaterial>("shadowMap_mat", "glProg_shadowMap", ResourceAccessType::INTERNAL);
}

void MaterialManager::pushMaterialToGPU(std::string name, ResourceAccessType accessType )
{
    if(accessType == ResourceAccessType::EXTERNAL)
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
    else
    {
        //try to found material name among meshes which have already been loaded
        assert( m_internalMaterials.find(name)!= m_internalMaterials.end()  );

        //if material isn't present in scene (hasn't already be pushed to gpu)
        if(m_internalMaterialCount[name] == 0)
        {
            //push the material to the gpu
           m_internalMaterials[name]->pushToGPU();
        }
        //update material use counter
        m_internalMaterialCount[name]++;
    }
}

void MaterialManager::popMaterialFromGPU(std::string name, ResourceAccessType accessType )
{
    if(accessType == ResourceAccessType::EXTERNAL)
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
    else
    {
        //try to found material name among meshes which have already been loaded
        assert( m_internalMaterials.find(name)!= m_internalMaterials.end() );

        //update material use counter
        m_internalMaterialCount[name]--;
        //if material has totaly disappeared from the sceen
        if(m_internalMaterialCount[name] == 0)
        {
           //pop the material to the gpu
           m_internalMaterials[name]->popFromGPU();
        }
    }

}

std::shared_ptr<Material> MaterialManager::getMaterial(std::string name, ResourceAccessType accessType )
{
    if(accessType == ResourceAccessType::EXTERNAL)
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
    else
    {
        if( m_internalMaterials.find(name)!= m_internalMaterials.end() )
        {
            return m_internalMaterials[name];
        }
        else
        {
            return std::shared_ptr<Material>();
        }
    }
}

bool MaterialManager::containsMaterial(std::string name, ResourceAccessType accessType )
{
    if(accessType == ResourceAccessType::EXTERNAL)
        return m_materials.find(name)!= m_materials.end();
    else
        return m_internalMaterials.find(name)!= m_internalMaterials.end();
}

//Specialization for skybox material (deals with cubeMaps)

template<>
std::shared_ptr<Material> MaterialManager::createMaterial<SkyboxMaterial>(std::string name, std::string programName, ResourceAccessType accessType )
{
    auto program = ResourceManager::getInstance().getProgram(programName);

    if (!program)
    {
        std::cout << "error when creating material with name : " << programName << " there is no program with this name" << std::endl;
        return nullptr;
    }

    //make a new material instance, with name [parameter : name], and with a program which name is programName
    auto newMat = std::make_shared<SkyboxMaterial>(program); //std::shared_ptr<Material>(new Material( program ));

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

template<>
std::shared_ptr<Material> te::MaterialManager::createMaterial<SkyboxMaterial>(std::string name, std::string programName, std::vector<std::string> imgNames, ResourceAccessType accessType )
{
    auto program = ResourceManager::getInstance().getProgram(programName);
    std::vector<std::shared_ptr<CubeMap>> images;
    for(auto imgName : imgNames)
    {
        images.push_back(ResourceManager::getInstance().getCubeMap(imgName));
    }

    if (!program)
    {
        std::cout << "error when creating material with name : " << programName << " there is no program with this name" << std::endl;
        return nullptr;
    }

    //make a new material instance, with name [parameter : name], and with a program which name is programName
    auto newMat = std::make_shared<SkyboxMaterial>(program, images); //std::shared_ptr<Material>(new Material( program, images ));

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
std::shared_ptr<Material> MaterialManager::createMaterial<SkyboxMaterial>(std::string name, std::string programName, std::vector<std::string> imgNames, std::vector<float> parameters, ResourceAccessType accessType )
{
    auto program = ResourceManager::getInstance().getProgram(programName);
    std::vector<std::shared_ptr<CubeMap>> images;
    for(auto imgName : imgNames)
    {
        images.push_back(ResourceManager::getInstance().getCubeMap(imgName));
    }

    if (!program)
    {
        std::cout << "error when creating material with name : " << programName << " there is no program with this name" << std::endl;
        return nullptr;
    }

    //make a new material instance, with name [parameter : name], and with a program which name is programName
    auto newMat = std::make_shared<SkyboxMaterial>(program, images, parameters); //std::shared_ptr<Material>(new Material( program, images ));

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
