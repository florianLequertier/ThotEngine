#include "thotEngine/World.hpp"
#include "thotEngine/prefabmanager.hpp"
#include <memory>

namespace te{

static float World::m_stepTime = 1.f/60.f;

World::World()
{
    m_content[typeid(Entity)] = std::make_shared< CArray<Entity> >();
    m_content[typeid(MeshRenderer)] = std::make_shared< CArray<MeshRenderer> >();
    m_content[typeid(Transform)] = std::make_shared<CArray<Transform> >();
    m_content[typeid(Camera)] = std::make_shared<CArray<Camera> >();
    m_content[typeid(PointLight)] = std::make_shared<CArray<PointLight> >();
    m_content[typeid(DirectionalLight)] = std::make_shared<CArray<DirectionalLight> >();
    m_content[typeid(RigidBody)] = std::make_shared<CArray<RigidBody> >();

    m_ptrToEntities = std::static_pointer_cast<CArray<Entity>>(m_content[typeid(Entity)]);
    m_ptrToMeshRenderers = std::static_pointer_cast<CArray<MeshRenderer>>(m_content[typeid(MeshRenderer)]);
    m_ptrToTransforms = std::static_pointer_cast<CArray<Transform>>(m_content[typeid(Transform)]);
    m_ptrToCameras = std::static_pointer_cast<CArray<Camera>>(m_content[typeid(Camera)]);
    m_ptrToPointLights = std::static_pointer_cast<CArray<PointLight>>(m_content[typeid(PointLight)]);
    m_ptrToDirectionalLights = std::static_pointer_cast<CArray<DirectionalLight>>(m_content[typeid(DirectionalLight)]);
    m_ptrToRigidBodies = std::static_pointer_cast<CArray<RigidBody>>(m_content[typeid(RigidBody)]);

}

World::~World()
{

}

void World::init()
{
    //init all scripts
    m_scriptSystem.init(m_ptrsToScripts);
}

float World::getStepTime()
{
    return m_stepTime;
}

void World::setStepTime(float delta)
{
    m_stepTime = delta;
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

ExternalHandler<Entity> World::instantiate()
{
    int index = std::static_pointer_cast<CArray<Entity>>(m_content[typeid(Entity)])->instantiate();
    return ExternalHandler<Entity>(&m_content, index);
}

ExternalHandler<Entity> World::instantiate(std::shared_ptr<Prefab> prefab)
{
    auto handler = instantiateNew<te::Entity>();

    prefab->make(handler, *this);

    return handler;
}

ExternalHandler<Entity> World::instantiate(std::string prefabName)
{
    if(PrefabManager::getInstance().contains(prefabName))
    {
        auto handler = instantiateNew<te::Entity>();

        PrefabManager::getInstance().make(prefabName, handler, *this);

        return handler;
    }
    else
    {
        std::cerr<<"error while instantiating the prefab \""<<prefabName<<"\". This name doesn't refer to a valid prefab."<<std::endl;
        return ExternalHandler<Entity>();
    }
}

void World::destroy(ExternalHandler<Entity> target)
{
    std::static_pointer_cast<CArray<Entity>>(m_content[typeid(Entity)])->remove(target);
}

void World::update()
{
    m_testSystem.update(m_ptrToEntities);

    m_scriptSystem.update(m_ptrsToScripts);
}

void World::render()
{
    //m_renderer.render(m_ptrToCameras->operator [](0), m_ptrToMeshRenderers, m_ptrToPointLights, m_ptrToDirectionalLights);
    m_renderer.deferred_render(m_ptrToCameras->operator [](0), m_ptrToMeshRenderers, m_ptrToPointLights, m_ptrToDirectionalLights);
}

}
