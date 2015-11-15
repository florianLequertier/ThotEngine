
#include "thotEngine/Material.hpp"

namespace te{

Material::Material(std::shared_ptr<GLProgram> program)
{
    //m_resourceManager = resourceManager;
    m_program = program;

    initUniforms();
}

Material::Material(std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image>> images)
{
    //m_resourceManager = resourceManager;
    m_program = program;

    for(auto img : images)
        m_images.push_back(img);

    initUniforms();
}

Material::~Material()
{

}

void Material::pushToGPU()
{
    for(auto img : m_images)
    {
        ResourceManager::getInstance().pushImageToGPU(img.lock()->getName());
    }
}

void Material::popFromGPU()
{
    for(auto img : m_images)
    {
        ResourceManager::getInstance().popImageFromGPU(img.lock()->getName());
    }
}

GLuint Material::getGlId() const
{
    return m_program.lock()->getId();
}

void Material::initUniforms()
{
    GLuint programId = m_program.lock()->getId();
    m_uniforms.push_back(glGetUniformLocation(programId, "u_MVMatrix"));
    m_uniforms.push_back( glGetUniformLocation(programId, "u_MVPMatrix"));
    m_uniforms.push_back( glGetUniformLocation(programId, "u_normalMatrix"));

    m_uniforms.push_back( glGetUniformLocation(programId, "u_diffuse"));
}

void Material::setUniforms(const glm::mat4& modelMat, const glm::mat4& worldMat, const glm::mat4& viewMat)
{
    GLuint glId = m_program.lock()->getId();

    //matrix uniforms
    glm::mat4 MV = viewMat * modelMat; //modelView
    glm::mat4 MVP = worldMat * viewMat * modelMat; //modelViewProjection
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MV));


    //set program uniforms...
    glUniformMatrix4fv( m_uniforms[0] , 1, false, &MV[0][0] );
    glUniformMatrix4fv( m_uniforms[1] , 1, false, &MVP[0][0] );
    glUniformMatrix4fv( m_uniforms[2] , 1, false, &NormalMatrix[0][0] );

    if(m_images.size() > 0)
        glUniform1i( m_uniforms[3], m_images[0].lock()->getId());
}

void Material::use()
{
    m_program.lock()->use();
}

}
