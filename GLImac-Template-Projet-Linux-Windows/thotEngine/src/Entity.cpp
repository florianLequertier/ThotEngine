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

void Entity::setHandler(std::shared_ptr<BaseCArray> user, int index)
{
    m_thisHandler = ExternalHandler<Entity>(user, index);
}

ExternalHandler<Entity> Entity::getHandler()
{
    return m_thisHandler;
}

}
