#include <memory>
#include "thotEngine/World.hpp"
#include "thotEngine/WindowManager.hpp"

#include "thotEngine/physicsimulation.hpp"

//forward
#include "thotEngine/World.hpp"

namespace te{
namespace physic{

std::shared_ptr<btDiscreteDynamicsWorld> PhysicSimulation::getPhysicWorld() const
{
    return m_physicWorld;
}

PhysicSimulation::PhysicSimulation()
{
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_broadPhase = new btDbvtBroadphase();
    m_sequentialImpulseConstraintSolver = new btSequentialImpulseConstraintSolver();

    m_physicWorld = std::shared_ptr<btDiscreteDynamicsWorld>( new btDiscreteDynamicsWorld(m_dispatcher, m_broadPhase, m_sequentialImpulseConstraintSolver, m_collisionConfiguration) );

    // debug :
    m_debugDrawer = new DebugDrawer();

    btIDebugDraw::DefaultColors debugColors;
    debugColors.m_aabb = btVector3(1,0,0);
    debugColors.m_activeObject = btVector3(0,1,0);
    debugColors.m_deactivatedObject = btVector3(0,0,1);

    m_debugDrawer->setDefaultColors(debugColors);
    m_debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawAabb|btIDebugDraw::DBG_DrawWireframe);
    m_physicWorld->setDebugDrawer(m_debugDrawer);
}

PhysicSimulation::~PhysicSimulation()
{
    if(m_collisionConfiguration != nullptr)
        delete m_collisionConfiguration;
    if(m_dispatcher != nullptr)
        delete m_dispatcher;
    if(m_broadPhase != nullptr)
        delete m_broadPhase;
    if(m_sequentialImpulseConstraintSolver != nullptr)
        delete m_sequentialImpulseConstraintSolver;

    if(m_debugDrawer != nullptr)
        delete m_debugDrawer;
}

void PhysicSimulation::init(World& world, std::shared_ptr<CArray<Collider>> ptrToColliders, std::shared_ptr<CArray<RigidBody>> ptrToRigidBodies)
{
//    for(int i = 0; i < ptrToColliders->size(); ++i)
//    {
//        ptrToColliders->parse(i).init(world);
//    }

//    for(int i = 0; i < ptrToRigidBodies->size(); ++i)
//    {
//        ptrToRigidBodies->parse(i).init(world);
//        ptrToRigidBodies->parse(i).addToPhysicWorld(m_physicWorld);
//    }
}

void PhysicSimulation::update()
{
    m_physicWorld->stepSimulation(World::getStepTime(), 10);
}

void PhysicSimulation::setGravity(glm::vec3 value)
{
    m_physicWorld->setGravity(btVector3(value.x, value.y, value.z));
}

glm::vec3 PhysicSimulation::getGravity() const
{
    btVector3 vec = m_physicWorld->getGravity();
    return glm::vec3(vec.x(), vec.y(), vec.z());
}

void PhysicSimulation::debugDraw(Camera& cam) const
{
    m_physicWorld->debugDrawWorld();
}


/////////////////////////////////////////////////
//////////////////DebugDrawer////////////////////
/////////////////////////////////////////////////

DebugDrawer::DebugDrawer(): m_debugMode(DBG_DrawWireframe | DBG_DrawAabb )
{
}

DebugDrawer::~DebugDrawer()
{

}

void DebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
//    glBegin(GL_LINES);

//    glVertex3f(from.x(), from.y(), from.z()); glColor3f(color.x(), color.y(), color.z());
//    glVertex3f(to.x(), to.y(), to.z()); glColor3f(color.x(), color.y(), color.z());

//    glEnd();

    float tmp[ 6 ] = { from.getX(), from.getY(), from.getZ(),
                  to.getX(), to.getY(), to.getZ() };

       glColor4f(color.getX(), color.getY(), color.getZ(), 1.0f);
       glVertexPointer( 3,
                   GL_FLOAT,
                   0,
                   &tmp );

       glPointSize( 5.0f );
       glDrawArrays( GL_POINTS, 0, 2 );
       glDrawArrays( GL_LINES, 0, 2 );

}

void DebugDrawer::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color)
{
    //nothing
}

void DebugDrawer::reportErrorWarning(const char *warningString)
{
    std::cerr<<warningString<<std::endl;
}

void DebugDrawer::draw3dText(const btVector3 &location, const char *textString)
{
    //nothing
}

void DebugDrawer::setDebugMode(int debugMode)
{
    m_debugMode = debugMode;
}

int DebugDrawer::getDebugMode() const
{
    return m_debugMode;
}

}
}
