
#include "thotEngine/Transform.hpp"
#include "thotEngine/glm.hpp"

namespace te{

Transform::Transform() : m_translation(0,0,0), m_scale(1,1,1)
{
    setRotation(m_rotation);
    setTranslation(m_translation);
    setScale(m_scale);
}

Transform::~Transform()
{

}

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

void Transform::setRotation( glm::vec3 eulerAngles)
{
    m_rotation = glm::quat(glm::orientate3(eulerAngles));
    m_rotationMat = glm::mat4_cast(m_rotation);

    m_forward = glm::vec3(m_rotationMat*glm::vec4(1,0,0,0));
    m_right = glm::vec3(m_rotationMat*glm::vec4(0,-1,0,0));
    m_up = glm::vec3(m_rotationMat*glm::vec4(0,0,1,0));

    computeModelMatrix();
}

void Transform::setRotation(float x, float y, float z)
{
    m_rotation = glm::quat(glm::orientate3(glm::vec3(x,y,z)));
    m_rotationMat = glm::mat4_cast(m_rotation);

    m_forward = glm::vec3(m_rotationMat*glm::vec4(1,0,0,0));
    m_right = glm::vec3(m_rotationMat*glm::vec4(0,-1,0,0));
    m_up = glm::vec3(m_rotationMat*glm::vec4(0,0,1,0));

    computeModelMatrix();
}

void Transform::setScale(float x, float y, float z)
{
    m_scale = glm::vec3(x,y,z);
    m_scaleMat = glm::scale(glm::mat4(1), m_scale);

    computeModelMatrix();
}

void Transform::setScale(float s)
{
    m_scale = glm::vec3(s);
    m_scaleMat = glm::scale(glm::mat4(1), m_scale);

    computeModelMatrix();
}

void Transform::setTranslation(glm::vec3 translation)
{
    m_translation = translation;
    m_translationMat = glm::translate(glm::mat4(1), m_translation);

    computeModelMatrix();
}

void Transform::setTranslation(float x, float y, float z)
{
    m_translation = glm::vec3(x,y,z);
    m_translationMat = glm::translate(glm::mat4(1), m_translation);

    computeModelMatrix();
}

void Transform::rotateAround(float angle, glm::vec3 axis)
{
    setRotation(m_rotation * glm::angleAxis(angle, axis));
}

void Transform::localRotateAround(float angle, glm::vec3 axis)
{
    setRotation(m_rotation * glm::angleAxis(angle, axis.x*m_forward + axis.y*(-m_right) + axis.z*m_up ));
}

void Transform::localRotateAround(float angle, float x, float y, float z)
{
    setRotation(m_rotation * glm::angleAxis(angle, x*m_forward + y*(-m_right) + z*m_up ));
}

void Transform::translate(glm::vec3 delta)
{
    setTranslation(m_translation + delta);
}

void Transform::localTranslate(glm::vec3 delta)
{
    translate(delta.x*m_forward + delta.y*(-m_right) + delta.z*m_up);
}

void Transform::translate(float x, float y, float z)
{
    setTranslation(m_translation + glm::vec3(x, y, z));
}

void Transform::localTranslate(float x, float y, float z)
{
    translate(x*m_forward + y*(-m_right) + z*m_up);
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
