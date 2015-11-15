#ifndef PREFAB_HPP
#define PREFAB_HPP

#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include "thotEngine/CArray.hpp"

namespace te{

//Forward
class World;

class Prefab
{
private:
    std::string m_name;

    std::function<void(ExternalHandler<Entity>,World&)> m_makeFunction;

public:
    Prefab(const std::string& prefabName);
    ~Prefab();

    std::string getName() const;
    void setName(std::string name);

    int componentCount() const;

    void setMakeFunction(std::function<void(ExternalHandler<Entity>, World&)> makeFunction);

    void make(ExternalHandler<Entity> handler, World& world);


};

}

#endif // PREFAB_HPP

