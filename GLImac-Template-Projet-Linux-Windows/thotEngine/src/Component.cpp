
#include "thotEngine/Component.hpp"
#include "thotEngine/Entity.hpp"
#include "thotEngine/Transform.hpp"

namespace te{

Component::Component(): m_name("component")
{

}

Component::~Component()
{

}

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

ExternalHandler<Transform> Component::transform() const
{
    return m_owner->getComponent<Transform>();
}


}

