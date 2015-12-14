#ifndef PARTICLE_HPP
#define PARTICLE_HPP


#include "glm.hpp"
#include <vector>
#include <cstdlib>

namespace te {
namespace particle{

class Particle{
private:

    glm::vec3 m_pos, m_speed;
    unsigned char m_r, m_g, m_b, m_a;
    float m_size, m_angle, m_weight;
    float m_life;
    float m_cameraDistance;
    int m_indexParticule; //particule physique
    glm::vec3 m_initialSpeed, m_initialPosition;
    float initSize, initMaxLife;

public:
    Particle();
    ~Particle();

    float getLife() const;
    glm::vec4 getColor() const;
    float getSize() const;
    float getAngle() const;
    float getWeight() const;
    glm::vec3 getSpeed() const;
    glm::vec3 getPosition() const;
    float getCameraDistance() const;
    int getIndexParticule() const;
    glm::vec3 getInitialSpeed()const;
    glm::vec3 getInitialPosition()const;
    float getInitMaxLife() const;
    float getInitSize() const;


    void setLife(float life);
    void setColor(glm::vec4 color);
    void setSize(float size);
    void setAngle(float angle);
    void setWeight(float weight);
    void setSpeed(glm::vec3 speed);
    void setPosition(glm::vec3 position);
    void setcameradistance(float distance);
    void setIndexParticule(int newIndex);
    void setInitialSpeed(glm::vec3 newInitSpeed);
    void setInitialPosition(glm::vec3 newInitPos);
    void setInitMaxLife(float newMaxLife);
    void setInitSize(float newSize);

    inline bool operator<(Particle& other){
        // Sort in reverse order : far particles drawn first.
        return this->getCameraDistance() > other.getCameraDistance();

    }

    inline bool operator>(Particle& other){
        // Sort in reverse order : far particles drawn first.
        return this->getCameraDistance() < other.getCameraDistance();

    }

};

}
}

#endif // PARTICLE_HPP

