#include "thotEngine/Component.hpp"

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

