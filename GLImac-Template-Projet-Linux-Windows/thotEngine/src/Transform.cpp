
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

glm::mat4 Transform::getModelMatrix() const
{
    return m_modelMat;
}

void Transform::setRotation(glm::quat rotation)
{
    m_rotation = rotation;
    m_rotationMat = glm::mat4_cast(m_rotation);

    m_forward = glm::vec3(m_rotationMat*glm::vec4(1,0,0,0));
    m_right = glm::vec3(m_rotationMat*glm::vec4(0,-1,0,0));
    m_up = glm::vec3(m_rotationMat*glm::vec4(0,0,1,0));

    computeModelMatrix();
}

void Transform::setScale(glm::vec3 scale)
{
    m_scale = scale;
    m_scaleMat = glm::scale(glm::mat4(1), m_scale);

    computeModelMatrix();
}

void Transform::setTranslation(glm::vec3 translation)
{
    m_translation = translation;
    m_translationMat = glm::translate(glm::mat4(1), m_translation);

    computeModelMatrix();
}

glm::vec3 Transform::getForward() const
{
    return m_forward;
}

glm::vec3 Transform::getUp() const
{
    return m_up;
}

glm::vec3 Transform::getRight() const
{
    return m_right;
}

void Transform::computeModelMatrix()
{
    m_modelMat = m_translationMat*m_rotationMat*m_scaleMat;
}

}
