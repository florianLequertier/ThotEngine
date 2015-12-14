#ifndef PARTICLEEMITTER_HPP
#define PARTICLEEMITTER_HPP

#include "CArray.hpp"
#include "Component.hpp"
#include "Material.hpp"
#include "particle.hpp"

namespace te{
namespace particle {


class ParticleMover
{
    virtual void update(std::vector<Particle>::iterator it, int length, float deltaTime, glm::vec3 cameraPosition = glm::vec3(0,0,0)) = 0;
};

class ParticleDistribution
{
    virtual void distribut(Particle* p);
};

class ParticleEmitter : public Component, public BaseWorldObject<ParticleEmitter>
{

private:

    std::string m_materialName;
    ResourceAccessType m_materialAccessType;

    std::weak_ptr<ParticleMaterial> m_materialPtr;

    //particles management
    std::shared_ptr<ParticleMover> m_particleMover;
    std::shared_ptr<ParticleDistribution> m_particleDistributor;

    int m_newParticules;
    int m_nbNewParticlesBySecond;
    std::vector<Particle> m_particlesContainer;
    int m_lastUsedParticle;
    int m_particuleCount;
    int m_maxParticleCount;

    GLubyte* m_particuleData_color;
    GLfloat* m_particuleData_positionSize;
    GLfloat m_g_vertex_buffer_data[20];

    GLuint m_vertexBuffer_colorParticule;
    GLuint m_vertexBuffer_positionParticule;
    GLuint m_vertexBuffer_modelParticule;
    GLuint vao;

    glm::vec3 position;
    float nbNewParticulePre;

public:
    ParticleEmitter(unsigned int maxParticleCount = 1000);
    ~ParticleEmitter();

    virtual void init(World& world) override;

    void pushToGPU(); //init meshPtr and materialPtr
    void popFromGPU();

    void setMesh(std::string mesh, ResourceAccessType accessType = ResourceAccessType::EXTERNAL);
    void setMaterial(std::string material, ResourceAccessType accessType = ResourceAccessType::EXTERNAL);

    GLuint getMaterialID() const;

    void draw() const;
    void render(const glm::mat4& projectionMat, const glm::mat4& viewMat);

    glm::mat4 getModelMatrix() const;


private:
    int findUnusedParticle();
    void setNewParticules(float deltaTime);
    int getNewParticules() const;
    virtual void simulateParticules(float delta, glm::vec3 cameraPosition);
    void simulateParticules_fire(float delta, glm::vec3 cameraPosition);
    void SortParticles();

    void initialyseVBOs();
    void initialyseVAO();
    void InitManager();
    void createNewParticules(float delai, int nombreNouvellesParticules = 10000);
    void createNewParticules_fire(float delai, int nombreNouvellesParticules = 5000);
    void updateVBOs();

public:

    void updateEmitter(float deltaTime, glm::vec3 cameraPosition);
    void drawParticules(glm::mat4 Projection, glm::mat4 View);
    void drawParticules(); //draw dans le container

    int getParticuleCount() const;
    int getMaxParticleCount() const;

};

}
}

#endif // PARTICLEEMITTER_HPP

