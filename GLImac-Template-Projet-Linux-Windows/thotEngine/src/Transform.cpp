
#include "thotEngine/Transform.hpp"
#include "thotEngine/glm.hpp"

namespace te{


const glm::vec3 Transform::m_forward = glm::vec3(0,0,1);
const glm::vec3 Transform::m_up = glm::vec3(0,1,0);
const glm::vec3 Transform::m_right = glm::vec3(1,0,0);



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

    m_localForward = glm::vec3(m_rotation*glm::vec4(m_forward,0));
    m_localRight = glm::vec3(m_rotation*glm::vec4(m_right,0));
    m_localUp = glm::vec3(m_rotation*glm::vec4(m_up,0));

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

    m_localForward = glm::vec3(m_rotation*glm::vec4(m_forward,0));
    m_localRight = glm::vec3(m_rotation*glm::vec4(m_right,0));
    m_localUp = glm::vec3(m_rotation*glm::vec4(m_up,0));

    computeModelMatrix();
}

void Transform::setRotation(float x, float y, float z)
{
    m_rotation = glm::quat(glm::orientate3(glm::vec3(x,y,z)));
    m_rotationMat = glm::mat4_cast(m_rotation);

    m_localForward = glm::vec3(m_rotation*glm::vec4(m_forward,0));
    m_localRight = glm::vec3(m_rotation*glm::vec4(m_right,0));
    m_localUp = glm::vec3(m_rotation*glm::vec4(m_up,0));

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
    setRotation( glm::angleAxis(angle, axis) * m_rotation );
}

void Transform::rotateAround(float angle, float x, float y, float z)
{
    setRotation( glm::angleAxis(angle, glm::vec3(x,y,z)) * m_rotation );
}

void Transform::localRotateAround(float angle, glm::vec3 axis)
{
    setRotation( glm::angleAxis(angle, axis.z*m_localForward + axis.x*m_localRight + axis.y*m_localUp ) *m_rotation);
}

void Transform::localRotateAround(float angle, float x, float y, float z)
{
    setRotation( glm::angleAxis(angle, z*m_localForward + x*m_localRight + y*m_localUp ) * m_rotation);
}

void Transform::translate(glm::vec3 delta)
{
    setTranslation(m_translation + delta);
}

void Transform::localTranslate(glm::vec3 delta)
{
    translate(delta.z*m_localForward + delta.x*m_localRight + delta.y*m_localUp);
}

void Transform::translate(float x, float y, float z)
{
    setTranslation(m_translation + glm::vec3(x, y, z));
}

void Transform::localTranslate(float x, float y, float z)
{
    translate(z*m_localForward + x*m_localRight + y*m_localUp);
}

glm::vec3 Transform::getForward()
{
    return m_forward;
}

glm::vec3 Transform::getUp()
{
    return m_up;
}

glm::vec3 Transform::getRight()
{
    return m_right;
}

glm::vec3 Transform::getLocalForward() const
{
    return m_localForward;
}

glm::vec3 Transform::getLocalUp() const
{
    return m_localUp;
}

glm::vec3 Transform::getLocalRight() const
{
    return m_localRight;
}



void Transform::computeModelMatrix()
{
    m_modelMat = m_scaleMat*m_rotationMat*m_translationMat;
}

}
