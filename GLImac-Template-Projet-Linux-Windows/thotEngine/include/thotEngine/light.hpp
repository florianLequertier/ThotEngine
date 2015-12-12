#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "glm.hpp"
#include "gl/glew.h"
#include "Component.hpp"
#include "GLConfig.hpp"

namespace te{

class PointLight : public Component, public BaseWorldObject<PointLight>
{
private:
    glm::vec3 m_color;
    float m_radius;
    float m_intensity;

public:
    PointLight();
    PointLight(const glm::vec3& position, const glm::vec3& color, float radius);
    ~PointLight();
    virtual void init(World &world) override;

    glm::vec3 getPosition() const;
    glm::vec3 getColor() const;
    void setColor(const glm::vec3 &color);
    void setColor( float red, float green, float blue);
    float getRadius() const;
    void setRadius(float radius);
    float getIntensity() const;
    void setIntensity(float intensity);

};

class DirectionalLight : public Component, public BaseWorldObject<DirectionalLight>
{
private:
    glm::vec3 m_direction;
    glm::vec3 m_color;
    float m_intensity;

    GLuint m_depthMapFBO;
    GLuint m_depthMap;

public:
    DirectionalLight();
    DirectionalLight(const glm::vec3& direction, const glm::vec3& color);
    ~DirectionalLight();
    virtual void init(World& world) override;

    glm::vec3 getDirection() const;
    void setDirection(const glm::vec3 &direction);
    glm::vec3 getColor() const;
    void setColor(const glm::vec3 &color);
    void setColor( float red, float green, float blue);
    float getIntensity() const;
    void setIntensity(float intensity);

    //depthBuffer draw
    void bindDepthBuffer(glm::mat4& lightProjection, glm::mat4 &lightVPMatrix);
    void unbindDepthBuffer();
};

}


#endif // LIGHT_HPP

