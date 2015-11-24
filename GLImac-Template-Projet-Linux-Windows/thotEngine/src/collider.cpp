#include "thotEngine/collider.hpp"

namespace te{
namespace physic{

Collider::Collider(ShapeType shapeType) : m_shapeType(shapeType), m_dimensions(0.5, 0.5, 0.5), m_height(1), m_radius(0.5)
{

}

Collider::~Collider()
{

}

void Collider::init()
{
    auto parent = transform();
    parent->addUpdatableTransform(getHandler());

    setParent(parent);
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

void Collider::setHandler(std::shared_ptr<BaseCArray> user, int index)
{
    m_thisHandler = ExternalHandler<Collider>(user, index);
}

ExternalHandler<Collider> Collider::getHandler()
{
    return m_thisHandler;
}

}
}

