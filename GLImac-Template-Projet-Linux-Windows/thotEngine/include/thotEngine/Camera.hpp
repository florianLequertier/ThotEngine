#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm.hpp"
#include "thotEngine/Component.hpp"

namespace te{

class Camera: public Component
{
public : enum CAMERA_MOVEMENT {FORWARD, BACKWARD, RIGHT, LEFT, UP, DOWN};

         enum ProjectionMode {ORTHO, PERSPECTIVE};

         struct PerspectiveParameters
         {
             float fovy;
             float aspect;
             float near;
             float far;
         };

         struct OrthoParameters
         {
             float left;
             float right;
             float bottom;
             float top;
         };


private:
    glm::mat4 m_viewMatrix;
    glm::mat4 m_worldMatrix;

    OrthoParameters m_orthoParameters;
    PerspectiveParameters m_perspectiveParameters;
    ProjectionMode m_projectionMode;
public :
    Camera();
    ~Camera();

    void updateViewMatrix();
    void setPerspectiveParameters(const float& fovy, const float& aspect, const float& near, const float& far);
    void setOrthoParameters(float left, float right, float bottom, float top);
    void setProjectionMode(ProjectionMode projectionMode);

    bool getProjectionMode() const;

    glm::mat4 getWorldMatrix() const;
    glm::mat4 getViewMatrix();

};

}

#endif // CAMERA_HPP
