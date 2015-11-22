#ifndef BOUNDINGSHAPE_HPP
#define BOUNDINGSHAPE_HPP

#include "glm.hpp"

namespace te{

class BoundingShape
{

private:

public:
    bool intersects(const BoundingShape& other) = 0;

};

class BoundingBox : public BoundingShape
{

private :

    glm::vec3 m_topFrontInit;
    glm::vec3 m_topBackInit;
    glm::vec3 m_topRightInit;
    glm::vec3 m_topLeftInit;

    glm::vec3 m_bottomFrontInit;
    glm::vec3 m_bottomBackInit;
    glm::vec3 m_bottomRightInit;
    glm::vec3 m_bottomLeftInit;

    glm::vec3 m_topFront;
    glm::vec3 m_topBack;
    glm::vec3 m_topRight;
    glm::vec3 m_topLeft;

    glm::vec3 m_bottomFront;
    glm::vec3 m_bottomBack;
    glm::vec3 m_bottomRight;
    glm::vec3 m_bottomLeft;

public:
}

}
#endif // BOUNDINGSHAPE_HPP

