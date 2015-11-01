#include "thotEngine/World.hpp"
#include <memory>

namespace te{

World::World()
{
    m_content[typeid(Component)] = std::make_shared< CArray<Component> >();

    m_ptrToComponents = std::static_pointer_cast<CArray<Component>>(m_content[typeid(Component)]);
}

World::~World()
{

}

void World::update()
{
    m_testSystem.update(m_ptrToComponents);
}

}
