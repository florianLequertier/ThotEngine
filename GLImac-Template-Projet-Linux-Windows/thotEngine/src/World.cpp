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
    m_content[typeid(MeshRenderer)] = std::make_shared< CArray<MeshRenderer> >();

    m_ptrToEntities = std::static_pointer_cast<CArray<Entity>>(m_content[typeid(Entity)]);
    m_ptrToMeshRenderers = std::static_pointer_cast<CArray<MeshRenderer>>(m_content[typeid(MeshRenderer)]);

    m_resourceManager = std::make_shared<ResourceManager>();
    m_materialManager = std::make_shared<MaterialManager>();
}

void World::pushToGPU()
{
    for(int i = 0; i < m_ptrToMeshRenderers->size(); ++i)
    {
        m_ptrToMeshRenderers->parse(i).pushToGPU();
    }
}

void World::popFromGPU()
{
    for(int i = 0; i < m_ptrToMeshRenderers->size(); ++i)
    {
        m_ptrToMeshRenderers->parse(i).popFromGPU();
    }
}

void World::update()
{
    m_testSystem.update(m_ptrToComponents);
}

}
