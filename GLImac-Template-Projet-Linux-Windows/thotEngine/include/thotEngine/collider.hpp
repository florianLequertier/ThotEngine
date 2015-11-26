#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "Bullet/btBulletCollisionCommon.h"

#include "Transform.hpp"

namespace te{
namespace physic{

class Collider : public Component, public BaseWorldObject<Collider>
{
public:
    enum ShapeType{BOX, SPHERE, CAPSULE};

private:

    ShapeType m_shapeType;

    float m_radius;
    float m_height;
    glm::vec3 m_dimensions;

    glm::vec3 m_origin; //translation from the center of parent

public:
    Collider(ShapeType shapeType = ShapeType::BOX);
    virtual ~Collider();
    virtual void init() override;

    ShapeType getShapeType() const;
    void setShapeType(const ShapeType &shapeType);

    btCollisionShape* makeShape();

    float getRadius() const;
    void setRadius(float radius);
    float getHeight() const;
    void setHeight(float height);
    glm::vec3 getDimensions() const;
    void setDimensions(const glm::vec3 &dimensions);
    void setDimensions(float w, float h, float d);

    glm::mat4 getModelMatrix();

    glm::vec3 getOrigin() const;
    void setOrigin(const glm::vec3& origin);
};

}
}

#endif // COLLIDER_HPP

