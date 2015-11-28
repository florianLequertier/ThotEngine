#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include "Bullet/btBulletCollisionCommon.h"
#include "Bullet/btBulletDynamicsCommon.h"

#include "motionstate.hpp"
//#include "Transform.hpp"
#include "Component.hpp"
#include "collider.hpp"

namespace te{

class Transform; //forward

namespace physic{

class RigidBody : public Component, public Transformable, public BaseWorldObject<RigidBody>
{

private:
    std::shared_ptr<TransformResolver> m_transformResolver;

    btScalar m_mass;
    btVector3 m_inertia;

    std::shared_ptr<btRigidBody> m_target;

    std::shared_ptr<btCompoundShape> m_shape;
    std::shared_ptr<MotionState> m_motionState;

    std::weak_ptr<btDiscreteDynamicsWorld> m_ptrToPhysicWorld;

public:
    RigidBody(float mass = 0, glm::vec3 inertia = glm::vec3(0,0,0));
    ~RigidBody();

    /**
     * initialized m_ptrToWorld based on the physical simulation world owns.
     *initialize rigidbody, by created a new rigidbody and place a reference to it in m_target. Then the target in send to the simulation (ie : btDynamicWorld).
     */
    virtual void init(World& world) override;

    /**
     * update the targeted rigidBody such that it has the same scale / position / translation that the entity transform.
     */
    virtual void updateTransform(const Transform &transform) override;

    /**
     * build a bullet rigidbody and send it to the btDynamicWorld
     * if m_target is in wolrd do nothing
     * to update a rigidbody, call popFromSimulation() and then pushToSimulation();
     */
    void pushToSimulation();

    /**
     * remove the target btRigidbody (if any) from the btDynamicWorld
     */
    void popFromSimulation();

    /**
     * delete the previous shape (if any), then make the multi shape, base on all the collider attached to this entity
     */
    void makeShape();

    /**
     * calculate mass, inertia, and motion state, then create appropriate btRigidBody. and place it in m_target parameter
     */
    void makeTarget();

    float getMass() const;
    void setMass(float mass);
    btVector3 getInertia() const;
    void setInertia(const glm::vec3 &inertia);
};

}
}

#endif // RIGIDBODY_HPP

