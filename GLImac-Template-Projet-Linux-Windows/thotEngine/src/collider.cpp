#include "thotEngine/collider.hpp"
//forward
#include "thotEngine/World.hpp"

namespace te{
namespace physic{

Collider::Collider(ShapeType shapeType) : m_shapeType(shapeType), m_dimensions(0.5, 0.5, 0.5), m_height(1), m_radius(0.5), m_origin(0,0,0)
{

}

Collider::~Collider()
{

}

void Collider::init(World& world)
{
    auto parent = transform();
    parent->addUpdatableTransform(getHandler());
}

Collider::ShapeType Collider::getShapeType() const
{
    return m_shapeType;
}

void Collider::setShapeType(const ShapeType &shapeType)
{
    m_shapeType = shapeType;
}

btCollisionShape* Collider::makeShape()
{
    switch(m_shapeType)
    {
        case ShapeType::BOX :
            return new btBoxShape(btVector3(m_dimensions.x, m_dimensions.y, m_dimensions.z));
        break;
        case ShapeType::SPHERE :
            return new btSphereShape(m_radius);
        break;
        case ShapeType::CAPSULE :
            return new btCapsuleShape(m_radius, m_height);
        break;
    }
}

float Collider::getRadius() const
{
    return m_radius;
}

void Collider::setRadius(float radius)
{
    m_radius = radius;
}

float Collider::getHeight() const
{
    return m_height;
}

void Collider::setHeight(float height)
{
    m_height = height;
}

glm::vec3 Collider::getDimensions() const
{
    return m_dimensions;
}

void Collider::setDimensions(const glm::vec3 &dimensions)
{
    m_dimensions = dimensions;
}

void Collider::setDimensions(float w, float h, float d)
{
    m_dimensions = glm::vec3(w,h,d);
}

glm::mat4 Collider::getModelMatrix()
{
    glm::vec3 colliderPosition = transform()->getTranslation() + m_origin;
    return glm::translate(glm::mat4(1), colliderPosition);
}

glm::vec3 Collider::getOrigin() const
{
    return m_origin;
}

void Collider::setOrigin(const glm::vec3 &origin)
{
    m_origin = origin;
}

}
}

