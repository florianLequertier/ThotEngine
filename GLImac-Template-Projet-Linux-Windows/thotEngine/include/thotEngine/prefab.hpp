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

    std::vector< std::shared_ptr<WorldObject> > m_components;

    std::function<void(ExternalHandler<Entity>,World&)> m_makeFunction;

public:
    Prefab();
    ~Prefab();

    std::string getName() const;
    void setName(std::string name);

    template<typename T>
    std::shared_ptr<T> addComponent();

    template<typename T>
    std::shared_ptr<T> getComponent();

    template<typename T>
    void removeComponent();

    int componentCount() const;

    void setMakeFunction(std::function<void(ExternalHandler<Entity>, World&)> makeFunction);

    void make(ExternalHandler<Entity> handler, World& world);


};





template<typename T>
std::shared_ptr<T> Prefab::getComponent()
{
    for(int i = 0; i < m_components.size(); ++i)
    {
        if( std::dynamic_pointer_cast<T>(m_components[i]) )
        {
            return m_components[i];
        }
    }
}

template<typename T>
std::shared_ptr<T> Prefab::addComponent()
{
    std::shared_ptr<T> newComponent = std::make_shared<T>();
    m_components.push_back(newComponent);
    return newComponent;
}

template<typename T>
void Prefab::removeComponent()
{
    const std::type_index& type = typeid(T);

    auto findIt = std::find_if(m_components.begin(), m_components.end(),
                               [type](std::shared_ptr<IHandler> a){ return (a->istypeof(type)); });

    if(findIt != m_components.end())
    {
        m_components.erase(findIt);
    }
}

}

#endif // PREFAB_HPP

