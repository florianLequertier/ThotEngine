
#include "thotEngine/motionstate.hpp"

namespace te{
namespace physic{

MotionState::MotionState(ExternalHandler<Transform> entityTransform)
{
    auto transformMat = entityTransform->getModelMatrix();
    m_physicTransform.setFromOpenGLMatrix(glm::value_ptr(transformMat));

    m_entityTransform = entityTransform;
}

MotionState::~MotionState()
{

}

void MotionState::getWorldTransform(btTransform& worldTransform) const
{
    worldTransform = m_physicTransform;
}

void MotionState::setWorldTransform(const btTransform& worldTransform)
{
    btQuaternion rotation =  worldTransform.getRotation();
    btVector3 translation = worldTransform.getOrigin();

    m_entityTransform->setRotation(glm::quat( rotation.x(), rotation.y(), rotation.z(), rotation.w()));
    m_entityTransform->setTranslation(translation.x(), translation.y(), translation.z());
}


}
}
