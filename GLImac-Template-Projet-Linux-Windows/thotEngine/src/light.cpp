
#include "thotEngine/light.hpp"

namespace te{

PointLight::PointLight(): m_position(0,0,0), m_color(1,1,1), m_radius(10)
{

}

PointLight::PointLight(const glm::vec3& position, const glm::vec3& color, float radius): m_position(position), m_color(color), m_radius(radius)
{

}

PointLight::~PointLight()
{

}

glm::vec3 PointLight::getPosition() const
{
    return m_position;
}

void PointLight::setPosition(const glm::vec3 &position)
{
    m_position = position;
}

glm::vec3 PointLight::getColor() const
{
    return m_color;
}

void PointLight::setColor(const glm::vec3 &color)
{
    m_color = color;
}

float PointLight::getRadius() const
{
    return m_radius;
}

void PointLight::setRadius(float radius)
{
    m_radius = radius;
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

}
