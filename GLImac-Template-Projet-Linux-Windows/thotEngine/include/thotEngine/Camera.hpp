#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <functional>
#include "glm.hpp"
#include "thotEngine/Component.hpp"
#include "skybox.hpp"

namespace te{

class Camera: public Component, public BaseWorldObject<Camera>
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

    Skybox m_skybox;
    bool m_useSkybox;

    std::function<void(Camera*)> m_clearFunction;

public :
    Camera();
    ~Camera();
    virtual void init(World& world) override;

    void updateViewMatrix();
    void setPerspectiveParameters(const float& fovy, const float& aspect, const float& near, const float& far);
    void setOrthoParameters(float left, float right, float bottom, float top);
    void setProjectionMode(ProjectionMode projectionMode);

    bool getProjectionMode() const;

    glm::mat4 getWorldMatrix() const;
    glm::mat4 getViewMatrix();

    void setUseSkybox(bool state);
    bool useSkybox() const;
    void setSkyboxMaterial(std::string materialName);

    void clearWithSkybox();
    void clearWithColor();
    void clear();

    void renderSkybox();
};

}

#endif // CAMERA_HPP
