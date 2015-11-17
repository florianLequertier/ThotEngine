#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <string>
#include "thotEngine/CArray.hpp"

namespace te{

//Forwards
class Entity;
class Transform;

class Component : public WorldObject
{

protected :
    std::string m_name;

    ExternalHandler<Entity> m_owner;

public :
    Component();
    ~Component();

    static bool isScriptable();

    std::string getName() const;
    void setName(std::string name);

    void setOwner(ExternalHandler<Entity> owner);

    template<typename T>
    ExternalHandler<T> getComponent();

    ExternalHandler<Transform> transform() const;

//operator overload
    bool operator<(const Component& other)
    {
        return m_name < other.m_name;
    }

};

}


//#include "thotEngine/Entity.hpp"

//namespace te
//{

//template<typename T>
//ExternalHandler<T> Component::getComponent()
//{
//    return m_owner->getComponent<T>();
//}

//ExternalHandler<Transform> Component::transform()
//{
//    return m_owner->getComponent<Transform>();
//}

//}

#endif // COMPONENT_HPP
