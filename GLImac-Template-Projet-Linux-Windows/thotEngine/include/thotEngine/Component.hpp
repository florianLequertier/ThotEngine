#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <string>
#include "thotEngine/CArray.hpp"

namespace te{

//Forwards
class Entity;
class Transform;

class Component
{

private :
    std::string m_name;

    Handler<Entity> m_owner;

public :
    std::string getName() const;
    void setName(std::string name);

    template<typename T>
    Handler<T> getComponent();

    Handler<Transform> transform();

};
}


#include "thotEngine/Entity.hpp"

namespace te
{

template<typename T>
Handler<T> Component::getComponent()
{
    return m_owner->getComponent<T>();
}

Handler<Transform> Component::transform()
{
    return m_owner->getComponent<Transform>();
}

}

#endif // COMPONENT_HPP
