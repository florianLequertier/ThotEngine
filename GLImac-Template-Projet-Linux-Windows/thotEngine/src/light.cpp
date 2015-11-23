#include "thotEngine/Transform.hpp"
#include "thotEngine/light.hpp"

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

void PointLight::init()
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

}

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& color): m_direction(direction), m_color(color)
{

}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::init()
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

}
