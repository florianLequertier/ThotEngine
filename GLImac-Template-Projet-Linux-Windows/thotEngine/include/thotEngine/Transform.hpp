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

    static const glm::vec3 m_forward;
    static const glm::vec3 m_up;
    static const glm::vec3 m_right;

    glm::vec3 m_localForward;
    glm::vec3 m_localUp;
    glm::vec3 m_localRight;

public :
    Transform();
    ~Transform();

//    void init();
//    void updateTransformables();

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

    static glm::vec3 getForward();
    static glm::vec3 getUp();
    static glm::vec3 getRight();

    glm::vec3 getLocalForward() const;
    glm::vec3 getLocalUp() const;
    glm::vec3 getLocalRight() const;

    void computeModelMatrix();

};

class Transformable{
public:
    virtual void updateTransform(const Transform& transform) = 0;
};

}

#endif // TRANSFORM_H
