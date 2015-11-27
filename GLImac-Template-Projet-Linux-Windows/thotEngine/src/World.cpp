#include "thotEngine/World.hpp"
#include "thotEngine/prefabmanager.hpp"
#include <memory>

namespace te{

float World::m_stepTime = 1.f/60.f;

World::World()
{
    m_content[typeid(Entity)] = std::make_shared< CArray<Entity> >();

    m_content[typeid(MeshRenderer)] = std::make_shared< CArray<MeshRenderer> >();

    m_content[typeid(Transform)] = std::make_shared<CArray<Transform> >();

    m_content[typeid(Camera)] = std::make_shared<CArray<Camera> >();

    m_content[typeid(PointLight)] = std::make_shared<CArray<PointLight> >();

    m_content[typeid(DirectionalLight)] = std::make_shared<CArray<DirectionalLight> >();

    m_content[typeid(physic::RigidBody)] = std::make_shared<CArray<physic::RigidBody> >();

    m_content[typeid(physic::Collider)] = std::make_shared<CArray<physic::Collider> >();


    m_ptrToEntities = std::static_pointer_cast<CArray<Entity>>(m_content[typeid(Entity)]);
    m_ptrToEntities->init(60);

    m_ptrToMeshRenderers = std::static_pointer_cast<CArray<MeshRenderer>>(m_content[typeid(MeshRenderer)]);
    m_ptrToMeshRenderers->init(20);

    m_ptrToTransforms = std::static_pointer_cast<CArray<Transform>>(m_content[typeid(Transform)]);
    m_ptrToTransforms->init(20);

    m_ptrToCameras = std::static_pointer_cast<CArray<Camera>>(m_content[typeid(Camera)]);
    m_ptrToCameras->init(5);

    m_ptrToPointLights = std::static_pointer_cast<CArray<PointLight>>(m_content[typeid(PointLight)]);
    m_ptrToPointLights->init(20);

    m_ptrToDirectionalLights = std::static_pointer_cast<CArray<DirectionalLight>>(m_content[typeid(DirectionalLight)]);
    m_ptrToDirectionalLights->init(20);

    m_ptrToRigidBodies = std::static_pointer_cast<CArray<physic::RigidBody>>(m_content[typeid(physic::RigidBody)]);
    m_ptrToRigidBodies->init(20);

    m_ptrToColliders = std::static_pointer_cast<CArray<physic::Collider>>(m_content[typeid(physic::Collider)]);
    m_ptrToColliders->init(20);


    //physical simultaion setup :
    m_physicSimulation = std::make_shared<physic::PhysicSimulation>();

}

World::~World()
{

}

void World::init()
{
    //init all scripts
    m_scriptSystem.init(*this, m_ptrsToScripts);
    //init physics
    m_physicSimulation->init(*this, m_ptrToColliders, m_ptrToRigidBodies);
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
    return ExternalHandler<Entity>(m_content[typeid(Entity)], index);
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

    m_physicSimulation->update();
}

void World::render()
{
    //m_renderer.render(m_ptrToCameras->operator [](0), m_ptrToMeshRenderers, m_ptrToPointLights, m_ptrToDirectionalLights);
    m_renderer.deferred_render(m_ptrToCameras->parse(0), m_ptrToMeshRenderers, m_ptrToPointLights, m_ptrToDirectionalLights);
    //additionnal draw call if debugging :
    if(m_debugMode)
        m_physicSimulation->debugDraw(m_ptrToCameras->parse(0));
}

void World::setDebugMode(bool state)
{
    m_debugMode = state;
}

bool World::getDebugMode() const
{
    return m_debugMode;
}

void World::setGravity(float x, float y, float z)
{
    m_physicSimulation->setGravity(glm::vec3(x,y,z));
}

glm::vec3 World::getGravity() const
{
    return m_physicSimulation->getGravity();
}

std::shared_ptr<physic::PhysicSimulation> World::getPhysicSimulation() const
{
    return m_physicSimulation;
}

}
