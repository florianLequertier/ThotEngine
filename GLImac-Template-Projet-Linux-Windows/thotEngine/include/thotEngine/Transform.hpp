#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glm.hpp"

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
    Transform();
    ~Transform();

    glm::quat getRotation() const;
    glm::vec3 getScale() const;
    glm::vec3 getTranslation() const;
    glm::mat4 getModelMatrix() const;

    void setRotation(glm::quat rotation);
    void setScale(glm::vec3 scale);
    void setTranslation(glm::vec3 translation);

    void setRotation( glm::vec3 eulerAngles);
    void setRotation(float x, float y, float z);
    void setScale(float x, float y, float z);
    void setScale(float s);
    void setTranslation(float x, float y, float z);

    void rotateAround(float angle, glm::vec3 axis);
    void rotateAround(float angle, float x, float y, float z);
    void localRotateAround(float angle, float x, float y, float z);
    void localRotateAround(float angle, glm::vec3 axis);
    void translate(glm::vec3 delta);
    void localTranslate(glm::vec3 delta);
    void translate(float x, float y, float z);
    void localTranslate(float x, float y, float z);

    glm::vec3 getForward() const;
    glm::vec3 getUp() const;
    glm::vec3 getRight() const;

    void computeModelMatrix();

};

}

#endif // TRANSFORM_H
