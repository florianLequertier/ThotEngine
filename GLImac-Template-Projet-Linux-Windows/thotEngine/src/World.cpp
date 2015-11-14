#include "thotEngine/World.hpp"
#include <memory>

namespace te{

World::World()
{
    m_content[typeid(Entity)] = std::make_shared< CArray<Entity> >();
    m_content[typeid(MeshRenderer)] = std::make_shared< CArray<MeshRenderer> >();
    m_content[typeid(Transform)] = std::make_shared<CArray<Transform> >();
    m_content[typeid(Camera)] = std::make_shared<CArray<Camera> >();

    m_ptrToEntities = std::static_pointer_cast<CArray<Entity>>(m_content[typeid(Entity)]);
    m_ptrToMeshRenderers = std::static_pointer_cast<CArray<MeshRenderer>>(m_content[typeid(MeshRenderer)]);
    m_ptrToTransforms = std::static_pointer_cast<CArray<Transform>>(m_content[typeid(Transform)]);
    m_ptrToCameras = std::static_pointer_cast<CArray<Camera>>(m_content[typeid(Camera)]);

}

World::~World()
{

}

void World::init()
{
    //init all scripts
    m_scriptSystem.init(m_ptrsToScripts);
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

ExternalHandler<Entity> World::instantiate()
{
    int index = std::static_pointer_cast<CArray<Entity>>(m_content[typeid(Entity)])->instantiate();
    return ExternalHandler<Entity>(&m_content, index);
}

ExternalHandler<Entity> World::instantiate(std::shared_ptr<Prefab> prefab)
{
    int index = std::static_pointer_cast<CArray<Entity>>(m_content[typeid(Entity)])->instantiate();
    ExternalHandler<Entity> handler(&m_content, index);

    for(int i = 0; i < prefab->componentCount(); ++i)
    {
        std::shared_ptr<WorldObject> newComponent;
        attachTo<decltype(*newComponent)>( handler, std::static_pointer_cast<decltype(newComponent)>(newComponent) );
    }

    return handler;
}

void World::update()
{
    m_testSystem.update(m_ptrToEntities);

    m_scriptSystem.update(m_ptrsToScripts);
}

void World::render()
{
    m_renderer.render(m_ptrToCameras->operator [](0), m_ptrToMeshRenderers);
}

}
