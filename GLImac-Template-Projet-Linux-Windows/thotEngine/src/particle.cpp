#include <GL/glew.h>
#include <algorithm>

#include "thotEngine/particle.hpp"

namespace te {
namespace particle{

    Particle::Particle() :m_pos(glm::vec3(0.f, 0.f, 0.f)), m_speed(glm::vec3(0.f, 0.f, 0.f)), m_r(0), m_g(0), m_b(0), m_a(0), m_size(0.f), m_angle(0.f), m_weight(0.f), m_life(0.f), m_cameraDistance(0.f), m_indexParticule(0)
    {

    }
    Particle::~Particle()
    {

    }

    float Particle::getLife() const
    {
        return m_life;
    }
    glm::vec4 Particle::getColor() const
    {
        return glm::vec4(m_r, m_g, m_b, m_a);
    }
    float Particle::getSize() const
    {
        return m_size;
    }
    float Particle::getAngle() const
    {
        return m_angle;
    }
    float Particle::getWeight() const
    {
        return m_weight;
    }
    glm::vec3 Particle::getSpeed() const
    {
        return m_speed;
    }
    glm::vec3 Particle::getPosition() const
    {
        return m_pos;
    }
    float Particle::getCameraDistance() const
    {
        return m_cameraDistance;
    }

    glm::vec3 Particle::getInitialSpeed()const
    {
        return m_initialSpeed;
    }
    glm::vec3 Particle::getInitialPosition()const
    {
        return m_initialPosition;
    }
    int Particle::getIndexParticule() const
    {
            return m_indexParticule;
    }
    float Particle::getInitMaxLife() const
    {
        return initMaxLife;
    }
    float Particle::getInitSize() const
    {
        return initSize;
    }


    void Particle::setLife(float life)
    {
        m_life = life;
    }
    void Particle::setColor(glm::vec4 color)
    {
        m_r = color.r;
        m_g = color.g;
        m_b = color.b;
        m_a = color.a;
    }
    void Particle::setSize(float size)
    {
        m_size = size;
    }
    void Particle::setAngle(float angle)
    {
        m_angle = angle;
    }
    void Particle::setWeight(float weight)
    {
        m_weight = weight;
    }
    void Particle::setSpeed(glm::vec3 speed)
    {
        m_speed = speed;
    }
    void Particle::setPosition(glm::vec3 position)
    {
        m_pos = position;
    }
    void Particle::setcameradistance(float distance)
    {
        m_cameraDistance = distance;
    }
    void Particle::setInitialSpeed(glm::vec3 newInitSpeed)
    {
        m_initialSpeed = newInitSpeed;
    }
    void Particle::setInitialPosition(glm::vec3 newInitPos)
    {
        m_initialPosition = newInitPos;
    }
    void Particle::setIndexParticule(int newIndex)
    {
            m_indexParticule = newIndex;
    }
    void Particle::setInitMaxLife(float newMaxLife)
    {
        initMaxLife = newMaxLife;
    }
    void Particle::setInitSize(float newSize)
    {
        initSize = newSize;
    }

}
}

