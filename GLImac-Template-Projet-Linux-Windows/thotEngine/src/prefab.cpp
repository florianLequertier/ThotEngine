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

void Prefab::setMakeFunction(std::function<void(ExternalHandler<Entity>, World&)> makeFunction)
{
    m_makeFunction = makeFunction;
}

void Prefab::make(ExternalHandler<Entity> handler, World& world)
{
    m_makeFunction(handler, world);
}

}
