#include "thotEngine/MaterialManager.hpp"
#include "thotEngine/particleemitter.hpp"

namespace te{

namespace particle{

ParticleEmitter::ParticleEmitter(unsigned int maxParticleCount) : m_maxParticleCount(maxParticleCount), m_nbNewParticlesBySecond(0), m_newParticules(0), m_lastUsedParticle(0), m_particuleCount(0), nbNewParticulePre(0.f)
{
    setMaterial("defaultParticle_mat", ResourceAccessType::INTERNAL);

    m_particlesContainer.resize(m_maxParticleCount);
    for (int i = 0; i < m_maxParticleCount; i++)
    {
        m_particlesContainer[i].setLife(-1.0f);
        m_particlesContainer[i].setcameradistance(-1.0f);
        m_particlesContainer[i].setColor(glm::vec4(0, 255, 0, 1));
        m_particlesContainer[i].setAngle((rand()%200)/100.f);
        glm::vec2 posInit(glm::diskRand(4.f));
        m_particlesContainer[i].setInitialPosition(glm::vec3(posInit.x, 0, posInit.y) + position);
        glm::vec2 speedInit(glm::diskRand(4.f));
        m_particlesContainer[i].setInitialSpeed(glm::vec3(speedInit.x, 0, speedInit.y));
        m_particlesContainer[i].setWeight(1.f);

        m_particlesContainer[i].setSpeed(glm::vec3(speedInit.x, 0, speedInit.y));
        m_particlesContainer[i].setPosition(glm::vec3(posInit.x, 0, posInit.y) + position);
        m_particlesContainer[i].setSize(0.f);
        m_particlesContainer[i].setInitSize(1.f);

    }

    m_particuleData_color = new GLubyte[m_maxParticleCount * 4];
    m_particuleData_positionSize = new GLfloat[m_maxParticleCount * 4];

    /*
    for (int i = 0; i<MAXPARTICULE; i++){
        m_particlesContainer[i]->setLife(-1.0f);
        m_particlesContainer[i]->setcameradistance(-1.0f);
        m_particlesContainer[i]->setColor(glm::vec4(0, 255, 0, 1));
    }*/

    //initiaisation des VBOs stockés dans l'objet :
    initialyseVBOs();
    initialyseVAO();
}

ParticleEmitter::~ParticleEmitter()
{
    popFromGPU();

    delete(m_particuleData_color);
    delete(m_particuleData_positionSize);

    glDeleteBuffers(1, &m_vertexBuffer_colorParticule);
    glDeleteBuffers(1, &m_vertexBuffer_positionParticule);
    glDeleteBuffers(1, &m_vertexBuffer_modelParticule);
    glDeleteVertexArrays(1, &vao);
}

void ParticleEmitter::init(World &world)
{
    //nothing
}

void ParticleEmitter::pushToGPU()
{
    MaterialManager::getInstance().pushMaterialToGPU(m_materialName, m_materialAccessType);
}

void ParticleEmitter::popFromGPU()
{
    MaterialManager::getInstance().popMaterialFromGPU(m_materialName, m_materialAccessType);
}

GLuint ParticleEmitter::getMaterialID() const
{
    return m_materialPtr.lock()->getGlId();
}

void ParticleEmitter::setMaterial(std::string material, ResourceAccessType accessType)
{
    m_materialName = material;
    m_materialAccessType = accessType;
    m_materialPtr = std::static_pointer_cast<ParticleMaterial>( MaterialManager::getInstance().getMaterial(m_materialName, m_materialAccessType) );

    //push graphics :
    MaterialManager::getInstance().pushMaterialToGPU(m_materialName, m_materialAccessType);
}

void ParticleEmitter::draw() const
{
    glBindVertexArray(vao);

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_particuleCount);

    glBindVertexArray(0);
}

void ParticleEmitter::render(const glm::mat4& projectionMat, const glm::mat4& viewMat)
{
    glm::vec3 cameraRight = viewMat[0]; // first column of view matrix = right vector
    glm::vec3 cameraUp = viewMat[1]; // second column of view matrix = up vector

    //set uniforms of the current material
    m_materialPtr.lock()->setUniforms(projectionMat, viewMat, cameraRight, cameraUp);

    //call opengl draw call
    draw();
}

glm::mat4 ParticleEmitter::getModelMatrix() const
{
    return m_owner->getComponent<Transform>()->getModelMatrix();
}


/*
    Cette fonction est à appeler à chaque frame et gére l'animation du sytème de particule (vie et mort des particules, simulation physique...)
*/
void ParticleEmitter::updateEmitter(float deltaTime, glm::vec3 cameraPosition)
{
    if(m_particleMover)
    {
        m_particleMover->update( m_particlesContainer.iterator() , deltaTime, cameraPosition);
    }
}

/*
    Definie le nombre de particule à faire apparaitre en un tour de boucle, en fonction du framerate.
*/

void ParticleEmitter::setNewParticules(float deltaTime)
{
    //float baseNumberf = baseNumber/1000.f;
    float nbNewParticleTotal = deltaTime*m_nbNewParticlesBySecond;
    m_newParticules = (int)(nbNewParticleTotal);
    nbNewParticulePre += (nbNewParticleTotal - m_newParticules);

    while(nbNewParticulePre > 1)
    {
        nbNewParticulePre--;
        m_newParticules++;
    }

    else if (m_newParticules >= m_maxParticleCount - m_particuleCount)
        m_newParticules = (m_maxParticleCount - m_particuleCount);
}

//simple getter sur m_newParticule (nombre de particule à creer dans la boucle principale)
int ParticleEmitter::getNewParticules() const
{
    return m_newParticules;
}

/*
    Créé des nouvelles particules, qui ne demandent enssuite plus qu'à être simulée avec simulateParticules.
*/
void ParticleEmitter::createNewParticules(float delai)
{

    //definie le nombre de particule à creer :
    setNewParticules(delai);

    for (int i = 0; i<m_newParticules; i++){

        int particleIndex = findUnusedParticle();

        if(m_particleDistributor)
        m_particleDistributor->distribut(&m_particlesContainer[particleIndex]);

//        fait par le ParticleDistributor

//        m_particlesContainer[particleIndex]->setLife(5.0f); // This particle will live 5 seconds.
//        m_particlesContainer[particleIndex]->setcameradistance(-1.0f);
//        m_particlesContainer[particleIndex]->setColor(glm::vec4(0, 255, 0, 1));
//        m_particlesContainer[particleIndex]->setAngle((rand()%200)/100.f);
//        glm::vec2 posInit(glm::diskRand(1.f));
//        m_particlesContainer[particleIndex]->setInitialPosition(glm::vec3(posInit.x, 0, posInit.y) + position);
//        glm::vec2 speedInit(glm::diskRand(1.f));
//        m_particlesContainer[particleIndex]->setInitialSpeed(glm::vec3(speedInit.x, 0, speedInit.y));
//        m_particlesContainer[particleIndex]->setWeight(1.f);



//        m_particlesContainer[particleIndex]->setSpeed(glm::vec3(speedInit.x, 0, speedInit.y));
//        m_particlesContainer[particleIndex]->setPosition(glm::vec3(posInit.x, 0, posInit.y) + position);


//        // Very bad way to generate a random color
//        m_particlesContainer[particleIndex]->setColor(glm::vec4(rand() % 256, rand() % 256, rand() % 256, (rand() % 256) / 3));

//        m_particlesContainer[particleIndex]->setSize((rand() % 1000) / 2000.0f + 0.1f);

    }

}


/*
    recherche d'une particule non vivante, si aucune particule n'est trouvé, renvoie la particule 0 = premiere particule simulée.
*/

int ParticleEmitter::findUnusedParticle()
{
    for (int i = m_lastUsedParticle; i<MAXPARTICULE; i++){
        if (m_particlesContainer[i]->getLife() < 0){
            m_lastUsedParticle = i;
            return i;
        }
    }

    for (int i = 0; i<m_lastUsedParticle; i++){
        if (m_particlesContainer[i]->getLife() < 0){
            m_lastUsedParticle = i;
            return i;
        }
    }

    return 0;
}

/*
    Fonction de trie permettant de trier les particule de m_particuleContainer en fonction de leur distance par rapport à la caméra.
*/
void ParticleEmitter::SortParticles()
{
    std::sort(&m_particlesContainer[0], &m_particlesContainer[MAXPARTICULE-1], [](Particle* a, Particle* b)->bool { return a->getCameraDistance() > b->getCameraDistance(); } );
}

// fait par le ParticleMover
/*
    Joue une simulation basique (une seule force : la gravité) sur l'ensemble des particules.
    Remplis les VBOs des positions, tailles et couleurs.
*/
void ParticleEmitter::simulateParticules(float delta, glm::vec3 cameraPosition)
{
    m_particuleCount = 0;

    for (int i = 0; i<MAXPARTICULE; i++){

        Particle& p = *m_particlesContainer[i]; // shortcut

        if (p.getLife() > 0.0f){

            // Decrease life
            p.setLife( p.getLife() - delta );
            if (p.getLife() > 0.0f){

                // Simulate simple physics : gravity only, no collisions
                p.setSpeed( p.getSpeed() + glm::vec3(0.0f, -9.81f, 0.0f) * (float)delta * 0.5f );
                p.setPosition(p.getPosition() + p.getSpeed() * (float)delta);
                p.setcameradistance(glm::length2(cameraPosition - p.getPosition())); //ou lenght2 ?
                //ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

                // Fill the GPU buffer
                m_particuleData_positionSize[4 * m_particuleCount + 0] = p.getPosition().x;
                m_particuleData_positionSize[4 * m_particuleCount + 1] = p.getPosition().y;
                m_particuleData_positionSize[4 * m_particuleCount + 2] = p.getPosition().z;

                m_particuleData_positionSize[4 * m_particuleCount + 3] = p.getSize();

                m_particuleData_color[4 * m_particuleCount + 0] = p.getColor().r;
                m_particuleData_color[4 * m_particuleCount + 1] = p.getColor().g;
                m_particuleData_color[4 * m_particuleCount + 2] = p.getColor().b;
                m_particuleData_color[4 * m_particuleCount + 3] = p.getColor().a;

                m_particuleCount++;
            }
            else{
                // Particles that just died will be put at the end of the buffer in SortParticles();
                p.setcameradistance( -1.0f );
            }

        }
    }

    //evite les erreurs d'affichage
    SortParticles();

    updateVBOs();

}



/*
    Initialisation des VBOs du programme.
    Un VBO pour stocker le model : 4 point pour un rectangle, donc 4*3 = 12 floats,
    un VBO pour stocker les donnés de position et de taille des particules,
    un VBO pour stocker les donnés couleur des particules.
*/


void ParticleEmitter::initialyseVBOs()
{
    //création des points du triangle :
    //position
    m_g_vertex_buffer_data[0] = 0.5f;
    m_g_vertex_buffer_data[1] = -0.5f;
    m_g_vertex_buffer_data[2] = 0.0f;
    //texCoord
    m_g_vertex_buffer_data[3] = 1.f;
    m_g_vertex_buffer_data[4] = 1.f;

    //position
    m_g_vertex_buffer_data[5] = -0.5f;
    m_g_vertex_buffer_data[6] = -0.5f;
    m_g_vertex_buffer_data[7] = 0.0f;
    //texCoord
    m_g_vertex_buffer_data[8] = 0.f;
    m_g_vertex_buffer_data[9] = 1.f;

    //position
    m_g_vertex_buffer_data[10] = 0.5f;
    m_g_vertex_buffer_data[11] = 0.5f;
    m_g_vertex_buffer_data[12] = 0.0f;
    //texCoord
    m_g_vertex_buffer_data[13] = 1.f;
    m_g_vertex_buffer_data[14] = 0.f;

    //position
    m_g_vertex_buffer_data[15] = -0.5f;
    m_g_vertex_buffer_data[16] = 0.5f;
    m_g_vertex_buffer_data[17] = 0.0f;
    //texCoord
    m_g_vertex_buffer_data[18] = 0.f;
    m_g_vertex_buffer_data[19] = 0.f;


    //creation du VBO du model de la particule :
    glGenBuffers(1, &m_vertexBuffer_modelParticule);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_modelParticule);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_g_vertex_buffer_data), m_g_vertex_buffer_data, GL_STATIC_DRAW);

    //creation du VBO contenant la position et la taille de la particule :
    glGenBuffers(1, &m_vertexBuffer_positionParticule);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_positionParticule);
    glBufferData(GL_ARRAY_BUFFER, m_maxParticleCount * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    //creation du VBO contenant les informatino de couleur de la particule :
    glGenBuffers(1, &m_vertexBuffer_colorParticule);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_colorParticule);
    glBufferData(GL_ARRAY_BUFFER, m_maxParticleCount * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

}

/*
    Met à jour les VBOs de position et de couleur.
    Cette fonction est utile car on dessine en stream les particules, on peut donc remplir rapidement les VBOs à la volée à une grande fréquence.
*/

void ParticleEmitter::updateVBOs()
{
    //update des buffers de position et de couleurs :
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_positionParticule);
    glBufferData(GL_ARRAY_BUFFER, m_maxParticleCount * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_particuleCount * 4 * sizeof(GLfloat), m_particuleData_positionSize);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_colorParticule);
    glBufferData(GL_ARRAY_BUFFER, m_maxParticleCount * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_particuleCount * 4 * sizeof(GLubyte), m_particuleData_color);

    //initialyseVAO();
}

void setMaxParticleCount(int maxCount)
{

}

/*
    Initialise les VAO, en faisant le lien entre les variable d'entré des shader et les VBOs du programe
*/

void ParticleEmitter::initialyseVAO()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //On lie ces VBO au VAO :
    //le VBO du model de la particule et des texture coordonnées (tableau de 4*3 + 4*2 floats) :
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_modelParticule);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)0);
    //le VBO du model de la particule (tableau de 6*3 floats) :
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));

    //le VBO des positions et tailles de toutes les particules (tableau de MAXPARTICULE * 4 floats) :
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_positionParticule);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    //le VBO des couleurs de toutes les particules (tableau de MAXPARTICULE * 4 GLubytes) :
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer_colorParticule);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);



    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

}

//alive particle count
int ParticleEmitter::getParticuleCount() const
{
    return m_particuleCount;
}

int ParticleEmitter::getMaxParticleCount() const
{
    return m_maxParticleCount;
}

}
}
