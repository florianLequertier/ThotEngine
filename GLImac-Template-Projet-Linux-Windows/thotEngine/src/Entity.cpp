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

std::string Entity::getName() const
{
    return m_name;
}

void Entity::setName(std::string name)
{
    m_name = name;
}

}
