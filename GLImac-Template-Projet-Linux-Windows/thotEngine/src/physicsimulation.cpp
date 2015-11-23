#include <memory>
#include "thotEngine/World.hpp"

#include "thotEngine/physicsimulation.hpp"

namespace te{
namespace physic{

PhysicSimulation::PhysicSimulation()
{
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_broadPhase = new btDbvtBroadphase();
    m_sequentialImpulseConstraintSolver = new btSequentialImpulseConstraintSolver();

    m_physicWorld = std::unique_ptr<btDiscreteDynamicsWorld>( new btDiscreteDynamicsWorld(m_dispatcher, m_broadPhase, m_sequentialImpulseConstraintSolver, m_collisionConfiguration) );
}

PhysicSimulation::~PhysicSimulation()
{
    delete m_collisionConfiguration;
    delete m_dispatcher;
    delete m_broadPhase;
    delete m_sequentialImpulseConstraintSolver;
}

void PhysicSimulation::init(std::shared_ptr<CArray<Collider>> ptrToColliders, std::shared_ptr<CArray<RigidBody>> ptrToRigidBodies)
{
    for(int i = 0; i < ptrToColliders->size(); ++i)
    {
        ptrToColliders->parse(i).init();
    }

    for(int i = 0; i < ptrToRigidBodies->size(); ++i)
    {
        ptrToRigidBodies->parse(i).init();
        ptrToRigidBodies->parse(i).addToPhysicWorld(*m_physicWorld.get());
    }
}

void PhysicSimulation::update()
{
    m_physicWorld->stepSimulation(World::getStepTime());
}

void PhysicSimulation::setGravity(glm::vec3 value)
{
    m_physicWorld->setGravity(btVector3(value.x, value.y, value.z));
}

glm::vec3 PhysicSimulation::getGravity() const
{
    btVector3 vec = m_physicWorld->getGravity();
    return glm::vec3(vec.x(), vec.y(), vec.z());
}

}
}
