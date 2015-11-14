#include <typeinfo>
#include <typeindex>

#include "thotEngine/Component.hpp"
#include "thotEngine/prefab.hpp"

namespace te {

Prefab::Prefab()
{

}

Prefab::~Prefab()
{

}

std::string Prefab::getName() const
{
    return m_name;
}

void Prefab::setName(std::string name)
{
    m_name = name;
}

int Prefab::componentCount() const
{
    return m_components.size();
}

}
