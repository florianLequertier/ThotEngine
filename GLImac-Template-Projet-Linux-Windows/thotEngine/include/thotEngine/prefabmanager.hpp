#ifndef PREFABMANAGER_HPP
#define PREFABMANAGER_HPP

#include <map>
#include <memory>
#include <string>

#include "CArray.hpp"

namespace te {

//forward
class Entity;
class World;
class Prefab;

class PrefabManager{

private:
    std::map<std::string, std::shared_ptr<Prefab>> m_container;

public:
    static PrefabManager& getInstance();
    ~PrefabManager();

    void add(std::shared_ptr<Prefab> prefab);
    void remove(std::string prefabName);
    void make(std::string prefabName, ExternalHandler<Entity> handler, World& world);
    bool contains(std::string prefabName);

private:
    PrefabManager();
    PrefabManager(const PrefabManager& other);
    PrefabManager(const PrefabManager&& other);
    PrefabManager& operator=(const PrefabManager& other);

};

}


#endif // PREFABMANAGER_HPP

