
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "thotEngine/Transform.hpp"

namespace te{

glm::quat Transform::getRotation() const
{
    return m_rotation;
}

glm::vec3 Transform::getScale() const
{
    return m_scale;
}

glm::vec3 Transform::getTranslation() const
{
    return m_translation;
}

glm::mat4 Transform::getModelMat() const
{
    return m_modelMat;
}

void Transform::setRotation(glm::quat rotation)
{
    m_rotation = rotation;
    m_rotationMat = glm::mat4_cast(m_rotation);

    computeModelMat();
}

void Transform::setScale(glm::vec3 scale)
{
    m_scale = scale;
    m_scaleMat = glm::scale(m_scaleValues);

    computeModelMat();
}

void Transform::setTranslation(glm::vec3 translation)
{
    m_translation = translation;
    m_translationMat = glm::translate(glm::mat4(1), m_translateVector);

    computeModelMat();
}

void Transform::computeModelMat()
{
    m_modelMat = m_translationMat*m_rotationMat*m_scaleMat;
}

}
