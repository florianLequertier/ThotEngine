#ifndef PHYSICSIMULATION_HPP
#define PHYSICSIMULATION_HPP

#include <memory>

#include "Bullet/btBulletCollisionCommon.h"
#include "Bullet/btBulletDynamicsCommon.h"

#include "CArray.hpp"
#include "rigidbody.hpp"

namespace te{
namespace physic{

class PhysicSimulation
{
private :
    btDefaultCollisionConfiguration m_collisionConfiguration;
    btCollisionDispatcher m_dispatcher;
    btDbvtBroadphase m_broadPhase;
    btSequentialImpulseConstraintSolver m_sequentialImpulseConstraintSolver;

    std::unique_ptr<btDiscreteDynamicsWorld> m_physicWorld;

public:
    PhysicSimulation();
    ~PhysicSimulation();

    void init(std::shared_ptr<CArray<RigidBody> > ptrToRigidBodies);
    void update();

    void setGravity(float value);
    float getGravity() const;

};

}
}

#endif // PHYSICSIMULATION_HPP

