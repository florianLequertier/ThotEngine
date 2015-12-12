#include "thotEngine/Transform.hpp"
#include "thotEngine/light.hpp"

//forward
#include "thotEngine/World.hpp"

namespace te{

PointLight::PointLight(): m_color(1,1,1), m_radius(10)
{

}

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, float radius): m_color(color), m_radius(radius)
{

}

PointLight::~PointLight()
{

}

void PointLight::init(World& world)
{
    //Nothing
}

glm::vec3 PointLight::getPosition() const
{
    return transform()->getTranslation();
}

glm::vec3 PointLight::getColor() const
{
    return m_color;
}

void PointLight::setColor(const glm::vec3 &color)
{
    m_color = color;
}

void PointLight::setColor( float red, float green, float blue)
{
    m_color = glm::vec3(red, green, blue);
}

float PointLight::getRadius() const
{
    return m_radius;
}

void PointLight::setRadius(float radius)
{
    m_radius = radius;
}

float PointLight::getIntensity() const
{
    return m_intensity;
}

void PointLight::setIntensity(float intensity)
{
    m_intensity = intensity;
}

/////// DirectionalLight ///////

DirectionalLight::DirectionalLight(): m_direction(1,0,0), m_color(1,1,1)
{
    //intiaialyze depth map :
    glGenFramebuffers(1, &m_depthMapFBO);

    glGenTextures(1, &m_depthMap);
    glBindTexture(GL_TEXTURE_2D, m_depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, gl::SHADOW_WIDTH, gl::SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& color): m_direction(direction), m_color(color)
{

}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::init(World &world)
{
    //Nothing
}

glm::vec3 DirectionalLight::getDirection() const
{
    return m_direction;
}

void DirectionalLight::setDirection(const glm::vec3 &direction)
{
    m_direction = direction;
}

glm::vec3 DirectionalLight::getColor() const
{
    return m_color;
}

void DirectionalLight::setColor(const glm::vec3 &color)
{
    m_color = color;
}

void DirectionalLight::setColor( float red, float green, float blue)
{
    m_color = glm::vec3(red, green, blue);
}

float DirectionalLight::getIntensity() const
{
    return m_intensity;
}

void DirectionalLight::setIntensity(float intensity)
{
    m_intensity = intensity;
}


//depthBuffer draw
void DirectionalLight::bindDepthBuffer( glm::mat4 &lightProjection, glm::mat4& lightVPMatrix)
{
    glViewport(0, 0, gl::SHADOW_WIDTH, gl::SHADOW_HEIGHT);

    glm::vec3 lightDirection = getDirection();
    glm::vec3 lightPosition = -lightDirection * 100.0f;

    glm::mat4 lightView = glm::lookAt(lightPosition, lightPosition + lightDirection, glm::vec3(0,1,0));
    lightVPMatrix = lightProjection * lightView;

    glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void DirectionalLight::unbindDepthBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}
