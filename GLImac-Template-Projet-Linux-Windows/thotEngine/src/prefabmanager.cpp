
#include "thotEngine/prefabmanager.hpp"
#include "thotEngine/World.hpp"
#include "thotEngine/Entity.hpp"
#include "thotEngine/prefab.hpp"

namespace te{

//singleton (c++11 implementation)
PrefabManager& PrefabManager::getInstance()
{
    static PrefabManager instance;
    return instance;
}

PrefabManager::~PrefabManager()
{

}

void PrefabManager::add(std::shared_ptr<Prefab> prefab)
{
    if(contains(prefab->getName()))
    {
        std::cout<<"warning : A prefab with the same name already exists. You will delete this prefab from the PrefabManager."<<std::endl;
    }
    m_container[prefab->getName()] = prefab;
}

void PrefabManager::remove(std::string prefabName)
{
    m_container.erase(prefabName);
}

void PrefabManager::make(std::string prefabName, ExternalHandler<Entity> handler, World& world)
{
    if(contains(prefabName))
    {
        m_container[prefabName]->make(handler, world);
    }
    else
    {
        std::cerr<<"error : You are trying to make an entity from a prefab which doesn't exist. The prefab with name : \""<<prefabName<<"\" doesn't exists.";
    }
}

bool PrefabManager::contains(std::string prefabName)
{
    if(m_container.find(prefabName) != m_container.end());
}

//privates
PrefabManager::PrefabManager(){}
PrefabManager::PrefabManager(const PrefabManager& other){}
PrefabManager::PrefabManager(const PrefabManager&& other){}
PrefabManager& PrefabManager::operator=(const PrefabManager& other){}

}
