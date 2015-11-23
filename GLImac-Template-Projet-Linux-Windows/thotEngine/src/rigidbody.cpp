#include "thotEngine/Transform.hpp"
#include "thotEngine/Entity.hpp"

#include "thotEngine/rigidbody.hpp"

namespace te{
namespace physic{

RigidBody::RigidBody(float mass, glm::vec3 inertia) : m_mass(mass), m_inertia(inertia.x, inertia.y, inertia.z), m_motionState(0), m_shape(0)
{

}

RigidBody::~RigidBody()
{

}

void RigidBody::init()
{
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
}

void RigidBody::updateTransform(const Transform &transform)
{

}

float RigidBody::getMass() const
{
    return m_mass;
}

void RigidBody::setMass(float mass)
{
    m_mass = mass;
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

void RigidBody::addToPhysicWorld(btDiscreteDynamicsWorld& physicWorld)
{
    physicWorld.addRigidBody(m_target);
}

}
}
