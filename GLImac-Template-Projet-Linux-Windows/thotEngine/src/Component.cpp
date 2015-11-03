
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

}

