#include "thotEngine/rigidbody.hpp"

namespace te{
namespace physic{

RigidBody::RigidBody(float mass, glm::vec3 inertia) : m_mass(mass), m_inertia(inertia)
{

}

RigidBody::~RigidBody()
{

}

void RigidBody::init()
{
    //make a shape
    std::vector<ExternalHandler<Collider>> colliders = getComponents<Collider>();
    btCompoundShape multiShape();

    for(auto collider : colliders)
    {
        btCollisionShape* childShape = collider.makeShape();
        btTransform childTransform;
        childTransform.setFromOpenGLMatrix(collider.getModelMatrix());

        multiShape.addChildShape(childTransform, childShape);
    }

    if(m_mass != 0)
        multiShape.calculateLocalInertia(m_mass, m_inertia);

    MotionState motionState(transform());

    btRigidBody::btRigidBodyConstructorInfo constructorInfo(m_mass, motionState , multiShape , m_inertia);

    m_target = new RigidBody(constructorInfo);
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

glm::vec3 RigidBody::getInertia() const
{
    return m_inertia;
}

void RigidBody::setInertia(const glm::vec3 &inertia)
{
    m_inertia = inertia;
}

void RigidBody::addToPhysicWorld(btDiscreteDynamicsWorld& physicWorld)
{
    physicWorld.addRigidBody(m_target);
}

}
}
