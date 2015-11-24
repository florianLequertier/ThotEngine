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

class RigidBody : public Component, public Transformable
{

private:
    btScalar m_mass;
    btVector3 m_inertia;

    btRigidBody* m_target;

    btCompoundShape* m_shape;
    MotionState* m_motionState;

public:
    RigidBody(float mass = 0, glm::vec3 inertia = glm::vec3(0,0,0));
    ~RigidBody();

    virtual void init() override;

    void updateTransform(const Transform &transform);

    float getMass() const;
    void setMass(float mass);
    btVector3 getInertia() const;
    void setInertia(const glm::vec3 &inertia);

    void addToPhysicWorld(btDiscreteDynamicsWorld& physicWorld);
};

}
}

#endif // RIGIDBODY_HPP

