#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "Bullet/btBulletCollisionCommon.h"

#include "Transform.hpp"

namespace te{
namespace physic{

class Collider : public Transform
{
public:
    enum ShapeType{BOX, SPHERE, CAPSULE};

private:
    using Transform::init;
    using Transform::updateTransformables;

    ShapeType m_shapeType;

    float m_radius;
    float m_height;
    glm::vec3 m_dimensions;

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
};

}
}

#endif // COLLIDER_HPP

