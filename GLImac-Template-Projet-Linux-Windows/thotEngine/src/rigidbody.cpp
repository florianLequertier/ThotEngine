#include "thotEngine/Transform.hpp"
#include "thotEngine/Entity.hpp"

#include "thotEngine/rigidbody.hpp"

//forward
#include "thotEngine/World.hpp"

namespace te{
namespace physic{

RigidBody::RigidBody(float mass, glm::vec3 inertia) : m_mass(mass), m_inertia(inertia.x, inertia.y, inertia.z), m_motionState(0), m_shape(0), m_target(nullptr)
{

}

RigidBody::~RigidBody()
{

}

void RigidBody::init(World &world)
{
    m_ptrToPhysicWorld = world.getPhysicSimulation()->getPhysicWorld();

    if(m_target != nullptr)
    {
        m_ptrToPhysicWorld.lock()->removeRigidBody(m_target);
        delete m_target;
        m_target = nullptr;
    }

    //make a shape
    std::vector<ExternalHandler<Collider>> colliders = getComponents<Collider>();

    if(m_shape != nullptr)
        delete m_shape;
    m_shape = new btCompoundShape();

    for(auto collider : colliders)
    {
        btCollisionShape* childShape = collider->makeShape();
        btTransform childTransform;
        childTransform.setFromOpenGLMatrix(glm::value_ptr(collider->getModelMatrix()));

        m_shape->addChildShape(childTransform, childShape);
    }

    if(m_mass != 0)
        m_shape->calculateLocalInertia(m_mass, m_inertia );

    if(m_motionState != nullptr)
        delete m_motionState;

    m_motionState = new MotionState(transform());

    btRigidBody::btRigidBodyConstructionInfo constructorInfo(m_mass, m_motionState , m_shape , m_inertia);

    m_target = new btRigidBody(constructorInfo);

    m_ptrToPhysicWorld.lock()->addRigidBody(m_target);

    updateTransform( *transform() );

}

void RigidBody::updateTransform(const Transform &transform)
{
    //update directly the rigidbody with the entity's transform
    btTransform newPhysicTransform;
    newPhysicTransform.setFromOpenGLMatrix( glm::value_ptr(transform.getModelMatrix()) );
    m_target->setWorldTransform(newPhysicTransform);
    //scale :
    glm::vec3 s = transform.getScale();
    m_target->getCollisionShape()->setLocalScaling( btVector3(s.x, s.y, s.z) );
    btVector3 newInertia;
    m_target->getCollisionShape()->calculateLocalInertia(m_mass, newInertia);
    m_target->setMassProps(m_mass, newInertia);
    m_target->updateInertiaTensor();

    m_ptrToPhysicWorld.lock()->updateSingleAabb(m_target);

}

float RigidBody::getMass() const
{
    return m_mass;
}

void RigidBody::setMass(float mass)
{
    m_mass = mass;

    btVector3 newInertia;
    m_target->getCollisionShape()->calculateLocalInertia(m_mass, newInertia);
    m_target->setMassProps(m_mass, newInertia);
    m_target->updateInertiaTensor();
    m_target->forceActivationState(DISABLE_DEACTIVATION);
    m_target->activate(true);
}

btVector3 RigidBody::getInertia() const
{
    return m_inertia;
}

void RigidBody::setInertia(const glm::vec3 &inertia)
{
    m_inertia.setX(inertia.x);
    m_inertia.setY(inertia.y);
    m_inertia.setZ(inertia.z);
}

void RigidBody::addToPhysicWorld(std::shared_ptr<btDiscreteDynamicsWorld> physicWorld)
{
    m_ptrToPhysicWorld = physicWorld;
    physicWorld->addRigidBody(m_target);
    //physicWorld->updateSingleAabb(m_target);
}

}
}
