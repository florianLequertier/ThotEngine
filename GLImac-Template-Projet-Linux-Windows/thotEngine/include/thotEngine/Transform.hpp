#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "thotEngine/Component.hpp"

namespace te{

class Transform : public Component
{

private :
    glm::quat m_rotation;
    glm::vec3 m_scale;
    glm::vec3 m_translation;

    glm::mat4 m_rotationMat;
    glm::mat4 m_scaleMat;
    glm::mat4 m_translationMat;

    glm::mat4 m_modelMat;

    glm::vec3 m_forward;
    glm::vec3 m_up;
    glm::vec3 m_right;

public :
    glm::quat getRotation() const;
    glm::vec3 getScale() const;
    glm::vec3 getTranslation() const;
    glm::mat4 getModelMatrix() const;

    void setRotation(glm::quat rotation);
    void setScale(glm::vec3 scale);
    void setTranslation(glm::vec3 translation);

    glm::vec3 getForward() const;
    glm::vec3 getUp() const;
    glm::vec3 getRight() const;

    void computeModelMatrix();

};

}

#endif // TRANSFORM_H
