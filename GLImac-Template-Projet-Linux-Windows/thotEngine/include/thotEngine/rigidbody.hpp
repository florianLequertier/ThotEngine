#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include "Bullet/btBulletCollisionCommon.h"
#include "Bullet/btBulletDynamicsCommon.h"

#include "motionstate.hpp"
#include "Transform.hpp"
#include "Component.hpp"

namespace te{
namespace physic{


class RigidBody : public Component, public Transformable
{

private:
    float m_mass;
    glm::vec3 m_inertia;

    btRigidBody* m_target;

public:
    RigidBody(float mass = 0, glm::vec3 inertia = glm::vec3(0,0,0));
    ~RigidBody();

    virtual void init() override;

    void updateTransform(const Transform &transform);

    float getMass() const;
    void setMass(float mass);
    glm::vec3 getInertia() const;
    void setInertia(const glm::vec3 &inertia);

    void addToPhysicWorld(btDiscreteDynamicsWorld& physicWorld);
};

}
}

#endif // RIGIDBODY_HPP

