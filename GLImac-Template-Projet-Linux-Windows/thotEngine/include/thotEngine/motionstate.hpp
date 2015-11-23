#ifndef MOTIONSTATE_HPP
#define MOTIONSTATE_HPP

#include "Bullet/btBulletCollisionCommon.h"
#include "Transform.hpp"

namespace te{
namespace physic{

class MotionState : public btMotionState
{
private:
    btTransform m_physicTransform;
    ExternalHandler<Transform> m_entityTransform;

public:
    MotionState(ExternalHandler<Transform> entityTransform);
    virtual ~MotionState();


    //bullet motion state overides :
    virtual void getWorldTransform(btTransform& worldTransform) const override;
    virtual void setWorldTransform(const btTransform& worldTransform) override;
};

}
}

#endif // MOTIONSTATE_HPP

