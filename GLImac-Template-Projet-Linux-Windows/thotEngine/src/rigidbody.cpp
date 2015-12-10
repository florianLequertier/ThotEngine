#include "thotEngine/Transform.hpp"
#include "thotEngine/Entity.hpp"

#include "thotEngine/rigidbody.hpp"

//forward
#include "thotEngine/World.hpp"

namespace te{
namespace physic{

RigidBody::RigidBody(float mass, glm::vec3 inertia) : m_mass(mass), m_inertia(inertia.x, inertia.y, inertia.z), m_motionState(0), m_shape(0), m_target(nullptr)
{
    m_transformResolver = std::make_shared<TransformResolver>();
    m_transformResolver->setTarget( dynamic_cast<Transformable*>(this) );
}

RigidBody::~RigidBody()
{
    auto parent = transform();
    parent->removeUpdatableTransform(m_transformResolver);

    //remove target from physic simulation
    popFromSimulation();
}

//initialized m_ptrToWorld based on the physical simulation world owns.
//initialize rigidbody, by created a new rigidbody and place a reference to it in m_target. Then the target in send to the simulation (ie : btDynamicWorld).
void RigidBody::init(World &world)
{
    m_ptrToPhysicWorld = world.getPhysicSimulation()->getPhysicWorld();

    auto parent = transform();
    parent->addUpdatableTransform(m_transformResolver);

    if(m_target && m_target->isInWorld())
    {
        popFromSimulation();
    }

    //make a shape :
    makeShape();

    //make btRigidBody :
    makeTarget();

    //add target to simulation :
    pushToSimulation();

    //update position and scale of the targeted btRigidBody
    updateTransform( *transform() );

}

//update the targeted rigidBody such that it has the same scale / position / translation that the entity transform.
void RigidBody::updateTransform(const Transform &transform)
{
    //update directly the rigidbody with the entity's transform
    btTransform newPhysicTransform;
    newPhysicTransform.setFromOpenGLMatrix( glm::value_ptr(transform.getModelMatrix()) );
//    glm::quat rotation =  transform.getRotation();
//    glm::vec3 translation = transform.getTranslation();
//    newPhysicTransform.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
//    newPhysicTransform.setOrigin(btVector3(translation.x, translation.y, translation.z));

    if(m_target && m_target->isInWorld())
    {
        popFromSimulation();
    }

    m_target->setWorldTransform(newPhysicTransform);
    //m_target->translate(btVector3(0,0,0));
    //m_target->translate(btVector3(translation.x, translation.y, translation.z));

    //scale :
    glm::vec3 s = transform.getScale();
    //m_target->getCollisionShape()->setLocalScaling( btVector3(s.x, s.y, s.z) );
    btVector3 newInertia;
    m_target->getCollisionShape()->calculateLocalInertia(m_mass, newInertia);
    m_target->setMassProps(m_mass, newInertia);
    m_target->updateInertiaTensor();

    //add target to simulation :
    pushToSimulation();

    //m_ptrToPhysicWorld.lock()->updateSingleAabb(m_target.get());
}

//build a bullet rigidbody and send it to the btDynamicWorld
//if m_target is in wolrd do nothing
//to update a rigidbody, call popFromSimulation() and then pushToSimulation();
void RigidBody::pushToSimulation()
{
    //target already pushed to world
    if(m_target!= nullptr && m_target->isInWorld())
        return;

    //add it to the world :
    m_ptrToPhysicWorld.lock()->addRigidBody(m_target.get());
}

//remove the target btRigidbody (if any) from the btDynamicWorld
void RigidBody::popFromSimulation()
{
    //remove btRigidBody from the world :
    if(m_target->isInWorld())
    {
        m_ptrToPhysicWorld.lock()->removeRigidBody(m_target.get());
    }
}

//delete the previous shape (if any), then make the multi shape, base on all the collider attached to this entity
void RigidBody::makeShape()
{
    //make a shape
    std::vector<ExternalHandler<Collider>> colliders = getComponents<Collider>();

    m_shape = std::make_shared<btCompoundShape>();

    for(auto collider : colliders)
    {
        btCollisionShape* childShape = collider->makeShape();
        btTransform childTransform;
        childTransform.setFromOpenGLMatrix(glm::value_ptr(collider->getModelMatrix()));

        m_shape->addChildShape(childTransform, childShape);
    }
}

//calculate mass, inertia, and motion state, then create appropriate btRigidBody. and place it in m_target parameter
void RigidBody::makeTarget()
{
    //calculate mass, motion state and inertia :
    if(m_mass != 0)
        m_shape->calculateLocalInertia(m_mass, m_inertia );

    m_motionState = std::make_shared<MotionState>(transform());

    btRigidBody::btRigidBodyConstructionInfo constructorInfo(m_mass, m_motionState.get() , m_shape.get() , m_inertia);

    //create a btRigidBody base on the previous infos :
    m_target = std::make_shared<btRigidBody>(constructorInfo);
}

float RigidBody::getMass() const
{
    return m_mass;
}

void RigidBody::setMass(float mass)
{
    m_mass = mass;

    popFromSimulation();

    btVector3 newInertia;
    m_target->getCollisionShape()->calculateLocalInertia(m_mass, newInertia);
    m_target->setMassProps(m_mass, newInertia);
    m_target->updateInertiaTensor();
    //m_target->forceActivationState(DISABLE_DEACTIVATION);
    m_target->activate(true);

    pushToSimulation();
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

}
}
