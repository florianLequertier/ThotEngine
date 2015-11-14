#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <vector>
#include <memory>
#include <algorithm>
#include "thotEngine/CArray.hpp"
#include "thotEngine/Transform.hpp"

namespace te{

//Forward
class World;

class Entity : public WorldObject
{
private:
    std::string m_name;

    std::vector<ExternalHandler<WorldObject>> m_components;

public:
    Entity();
    ~Entity();

    std::string getName() const;
    void setName(std::string name);

    template<typename T>
    ExternalHandler<T> addComponent(World& world);

    template<typename T>
    ExternalHandler<T> addComponent(World& world, T& model);

    template<typename T>
    ExternalHandler<T> removeComponent(World& world);

    template<typename T>
    ExternalHandler<T> getComponent();


//    template<typename T>
//    ExternalHandler<T> addComponent(World& worldptr, ExternalHandler<T> component);

    template<typename T>
    void addComponent(ExternalHandler<T> component);

    template<typename T>
    ExternalHandler<T> removeComponent();


//    template<typename T>
//    bool removeComponent(World& worldptr);

//operator overload
    bool operator<(const Entity& other)
    {
        return m_index < other.m_index;
    }


};

template<typename T>
ExternalHandler<T> Entity::getComponent()
{
    for(int i = 0; i < m_components.size(); ++i)
    {
        if(m_components[i].istypeof( typeid(T) ))
        {
            return m_components[i];
        }
    }
    return ExternalHandler<T>();
}

template<typename T>
void Entity::addComponent(ExternalHandler<T> component)
{
    m_components.push_back(component);
}

template<typename T>
ExternalHandler<T> Entity::removeComponent()
{
    const std::type_index& type = typeid(T);

    auto findIt = std::find_if(m_components.begin(), m_components.end(),
                               [type](std::shared_ptr<IHandler> a){ return (a->istypeof(type)); });

    if(findIt != m_components.end())
    {
        m_components.erase(findIt);
        return *std::static_pointer_cast<ExternalHandler<T>>(m_components.at(findIt));
    }
    else
        return *std::static_pointer_cast<ExternalHandler<T>>(m_components.at(findIt));
}

}

//forward declaration of Component
namespace te{

template<typename T>
ExternalHandler<T> Component::getComponent()
{
    return m_owner->getComponent<T>();
}

}

//#include "thotEngine/World.hpp"

//namespace te {

//template<typename T>
//ExternalHandler<T> Entity::addComponent(World& worldPtr)
//{
//    return worldPtr.attachTo<T>(thisHandler());
//}

//template<typename T>
//ExternalHandler<T> Entity::removeComponent(World& worldPtr)
//{
//    return worldPtr.removeFrom<T>(thisHandler());
//}

//}

#endif // ENTITY_HPP
