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

    m_physicWorld = std::make_shared<btDiscreteDynamicsWorld>(m_dispatcher, m_broadPhase, m_sequentialImpulseConstraintSolver, m_collisionConfiguration);
}

PhysicSimulation::~PhysicSimulation()
{

}

void PhysicSimulation::init(std::shared_ptr<CArray<RigidBody>> ptrToRigidBodies)
{
    for(int i = 0; i < ptrToRigidBodies.size(); ++i)
    {
        ptrToRigidBodies->parse(i).init();
        ptrToRigidBodies->addToSimulation(*this);
    }
}

void PhysicSimulation::update()
{
    m_physicWorld->stepSimulation(World::getStepTime());
}

void PhysicSimulation::setGravity(float value)
{

}

float PhysicSimulation::getGravity() const
{

}

}
}
