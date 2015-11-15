#include <GL/glew.h>
#include "thotEngine/CArray.hpp"
#include "thotEngine/Transform.hpp"

#include "thotEngine/Camera.hpp"

namespace te{

Camera::Camera()
{
    setPerspectiveParameters(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    setOrthoParameters(0, 800, 600, 0);
    setProjectionMode(ProjectionMode::PERSPECTIVE);
    m_clearFunction = &Camera::clearWithColor;
}

Camera::~Camera()
{

}

void Camera::updateViewMatrix()
{
    auto ownerTransform = transform();

    glm::vec3 translation = ownerTransform->getTranslation();
    glm::vec3 forward = ownerTransform->getForward();
    glm::vec3 up = ownerTransform->getUp();

    m_viewMatrix = glm::lookAt(
        ownerTransform->getTranslation(), // Camera is at (4,3,3), in World Space
        ownerTransform->getTranslation() + ownerTransform->getForward(), // and looks at the origin
        ownerTransform->getUp()  // Head is up (set to 0,-1,0 to look upside-down)
        );
}

void Camera::setPerspectiveParameters(const float& fovy, const float& aspect, const float& near, const float& far)
{
    m_perspectiveParameters.fovy = fovy;
    m_perspectiveParameters.aspect = aspect;
    m_perspectiveParameters.near = near;
    m_perspectiveParameters.far = far;
}

void Camera::setOrthoParameters(float left, float right, float bottom, float top)
{
    m_orthoParameters.left = left;
    m_orthoParameters.right = right;
    m_orthoParameters.bottom = bottom;
    m_orthoParameters.top = top;
}

void Camera::setProjectionMode(ProjectionMode projectionMode)
{
    m_projectionMode = projectionMode;

    if (m_projectionMode == ProjectionMode::PERSPECTIVE)
    {
        m_worldMatrix = glm::perspective(m_perspectiveParameters.fovy, m_perspectiveParameters.aspect, m_perspectiveParameters.near, m_perspectiveParameters.far);
    }
    else
    {
        m_worldMatrix = glm::ortho(m_orthoParameters.left, m_orthoParameters.right, m_orthoParameters.bottom, m_orthoParameters.top);
    }
}

bool Camera::getProjectionMode() const
{
    return m_projectionMode;
}

glm::mat4 Camera::getWorldMatrix() const
{
    return m_worldMatrix;
}

glm::mat4 Camera::getViewMatrix()
{
    updateViewMatrix();
    return m_viewMatrix;
}

void Camera::setUseSkybox(bool state)
{
    if(state && m_skybox.isInitialized())
    {
        m_useSkybox = true;
        m_clearFunction = &Camera::clearWithSkybox;
    }
    else
    {
        m_useSkybox = false;
        m_clearFunction = &Camera::clearWithColor;
    }
}

bool Camera::useSkybox() const
{
    return m_useSkybox;
}

void Camera::setSkyboxMaterial(std::string materialName)
{
    m_skybox.setMaterial(materialName);
}

void Camera::clearWithSkybox()
{
    auto ownerTransform = transform();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(m_useSkybox)
        m_skybox.render( glm::translate(ownerTransform->getTranslation()), m_worldMatrix, m_viewMatrix);
}

void Camera::clearWithColor()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Camera::clear()
{
    m_clearFunction(this);
}

}
