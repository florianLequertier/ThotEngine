#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "glm.hpp"
#include "Component.hpp"

namespace te{

class PointLight : public Component
{
private:
    glm::vec3 m_position;
    glm::vec3 m_color;
    float m_radius;

public:
    PointLight();
    PointLight(const glm::vec3& position, const glm::vec3& color, float radius);
    ~PointLight();

    glm::vec3 getPosition() const;
    void setPosition(const glm::vec3 &position);
    glm::vec3 getColor() const;
    void setColor(const glm::vec3 &color);
    float getRadius() const;
    void setRadius(float radius);
};

class DirectionalLight : public Component
{
private:
    glm::vec3 m_direction;
    glm::vec3 m_color;

public:
    DirectionalLight();
    DirectionalLight(const glm::vec3& direction, const glm::vec3& color);
    ~DirectionalLight();

    glm::vec3 getDirection() const;
    void setDirection(const glm::vec3 &direction);
    glm::vec3 getColor() const;
    void setColor(const glm::vec3 &color);
};

}


#endif // LIGHT_HPP

