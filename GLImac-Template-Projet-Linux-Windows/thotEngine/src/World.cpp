#include "thotEngine/World.hpp"
#include <memory>

namespace te{

World::World()
{

}

World::~World()
{

}

void World::init()
{
    m_content[typeid(Entity)] = std::make_shared< CArray<Entity> >();

    m_ptrToEntities = std::static_pointer_cast<CArray<Entity>>(m_content[typeid(Entity)]);
}

void World::update()
{
    m_testSystem.update(m_ptrToComponents);
}

}
