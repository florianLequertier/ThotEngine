
#include "thotEngine/Component.hpp"
#include "thotEngine/Entity.hpp"
#include "thotEngine/Transform.hpp"

namespace te{

std::string Component::getName() const
{
    return m_name;
}

void Component::setName(std::string name)
{
    m_name = name;
}

void Component::setOwner(ExternalHandler<Entity> owner)
{
    m_owner = owner;
}

ExternalHandler<Transform> Component::transform()
{
    return m_owner->getComponent<Transform>();
}

}

