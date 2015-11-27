#ifndef PHYSICSIMULATION_HPP
#define PHYSICSIMULATION_HPP

#include <memory>

#include "Bullet/btBulletCollisionCommon.h"
#include "Bullet/btBulletDynamicsCommon.h"
#include "Bullet/LinearMath/btIDebugDraw.h"

#include "CArray.hpp"
#include "rigidbody.hpp"
#include "Material.hpp"

namespace te{

//forward
class World;

namespace physic{

class DebugDrawer : public btIDebugDraw
{
private:
    int m_debugMode;

public :
    DebugDrawer();
    virtual ~DebugDrawer();

    void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color);
    void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color);
    void reportErrorWarning(const char *warningString);
    void draw3dText(const btVector3 &location, const char *textString);
    void setDebugMode(int debugMode);
    int getDebugMode() const;
};

class PhysicSimulation
{
private :
    btDefaultCollisionConfiguration* m_collisionConfiguration;
    btCollisionDispatcher* m_dispatcher;
    btDbvtBroadphase* m_broadPhase;
    btSequentialImpulseConstraintSolver* m_sequentialImpulseConstraintSolver;
    DebugDrawer* m_debugDrawer;

    std::shared_ptr<btDiscreteDynamicsWorld> m_physicWorld;

public:
    PhysicSimulation();
    ~PhysicSimulation();

    void init(World& world, std::shared_ptr<CArray<Collider> > ptrToColliders, std::shared_ptr<CArray<RigidBody> > ptrToRigidBodies);
    void update();

    void setGravity(glm::vec3 value);
    glm::vec3 getGravity() const;

    void debugDraw(te::Camera &cam) const;

    std::shared_ptr<btDiscreteDynamicsWorld> getPhysicWorld() const;
};

}
}

#endif // PHYSICSIMULATION_HPP

