#include <typeinfo>
#include <typeindex>

#include "thotEngine/Component.hpp"
#include "thotEngine/Entity.hpp"

namespace te {

Entity::Entity()
{

}

Entity::~Entity()
{

}

void Entity::init(int index)
{
    m_thisIndex = index;
}

Handler Entity::thisHandler()
{
    return Handler(typeid(Entity), m_thisIndex);
}

std::string Entity::getName() const
{
    return m_name;
}

void Entity::setName(std::string name)
{
    m_name = name;
}

}
