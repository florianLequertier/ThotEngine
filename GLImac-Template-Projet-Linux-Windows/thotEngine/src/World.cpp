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
}

//void World::init(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<MaterialManager> materialManager)
//{
//    m_content[typeid(Entity)] = std::make_shared< CArray<Entity> >();
//    m_content[typeid(MeshRenderer)] = std::make_shared< CArray<MeshRenderer> >();

//    m_ptrToEntities = std::static_pointer_cast<CArray<Entity>>(m_content[typeid(Entity)]);
//    m_ptrToMeshRenderers = std::static_pointer_cast<CArray<MeshRenderer>>(m_content[typeid(MeshRenderer)]);

//    m_resourceManager = resourceManager;
//    m_materialManager = materialManager;
//}

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
    m_testSystem.update(m_ptrToEntities);

    // TODO
    m_renderer.render(glm::mat4(1), glm::mat4(1), m_ptrToMeshRenderers);
}

}
