#include "thotEngine/GLConfig.hpp"

#include "thotEngine/Material.hpp"

namespace te{

Material::Material(std::shared_ptr<GLProgram> program)
{
    //m_resourceManager = resourceManager;
    m_program = program;
}

Material::Material(std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image>> images)
{
    //m_resourceManager = resourceManager;
    m_program = program;

    for(auto img : images)
        m_images.push_back(img);
}

Material::Material(std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image>> images, std::vector<float> parameters) : Material(program, images)
{

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

void Material::use()
{
    m_program.lock()->use();
}


//////////////////////////////////////////////////////////////////////////////
////////////////////////////// UnlitMaterial ////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

UnlitMaterial::UnlitMaterial( std::shared_ptr<GLProgram> program ) : Material(program)
{
    initUniforms();
}

UnlitMaterial::UnlitMaterial( std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image>> images ) : Material(program, images)
{
    initUniforms();
}

UnlitMaterial::UnlitMaterial( std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image>> images, std::vector<float> parameters) : UnlitMaterial(program, images)
{

}

UnlitMaterial::~UnlitMaterial()
{

}

void UnlitMaterial::initUniforms()
{
    GLuint programId = m_program.lock()->getId();
    m_uniforms.push_back(glGetUniformLocation(programId, "u_ModelMatrix"));
    m_uniforms.push_back( glGetUniformLocation(programId, "u_MVPMatrix"));
    m_uniforms.push_back( glGetUniformLocation(programId, "u_normalMatrix"));

    m_uniforms.push_back( glGetUniformLocation(programId, "u_diffuse"));
}

void UnlitMaterial::setUniforms(const glm::mat4& modelMat, const glm::mat4& worldMat, const glm::mat4& viewMat)
{
    GLuint glId = m_program.lock()->getId();

    //matrix uniforms
    glm::mat4 MV = viewMat * modelMat;
    glm::mat4 MVP = worldMat * MV; //modelViewProjection
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MV));


    //set program uniforms...
    glUniformMatrix4fv( m_uniforms[0] , 1, false, &modelMat[0][0] );
    glUniformMatrix4fv( m_uniforms[1] , 1, false, &MVP[0][0] );
    glUniformMatrix4fv( m_uniforms[2] , 1, false, &NormalMatrix[0][0] );

    if(m_images.size() > 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_images[0].lock()->getId());
        glUniform1i( m_uniforms[3], 0);
    }
}

void UnlitMaterial::setUniforms(const glm::mat4& modelMat, const glm::mat4& projectionMat, const glm::mat4& viewMat, std::shared_ptr<CArray<PointLight>> pointLights, std::shared_ptr<CArray<DirectionalLight>> directionalLights, const glm::vec3& viewPosition)
{
    GLuint glId = m_program.lock()->getId();

    //matrix uniforms
    glm::mat4 MV = viewMat * modelMat;
    glm::mat4 MVP = projectionMat * MV; //modelViewProjection
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MV));


    //set program uniforms...
    glUniformMatrix4fv( m_uniforms[0] , 1, false, &modelMat[0][0] );
    glUniformMatrix4fv( m_uniforms[1] , 1, false, &MVP[0][0] );
    glUniformMatrix4fv( m_uniforms[2] , 1, false, &NormalMatrix[0][0] );

    if(m_images.size() > 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_images[0].lock()->getId());
        glUniform1i( m_uniforms[3], 0);
    }
}

//////////////////////////////////////////////////////////////////////////////
/////////////////////////////// LitMaterial /////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


LitMaterial::LitMaterial( std::shared_ptr<GLProgram> program ) : Material(program)
{
    initUniforms();
}

LitMaterial::LitMaterial( std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image>> images ) : Material(program, images)
{
    initUniforms();
}

LitMaterial::LitMaterial( std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image>> images, std::vector<float> parameters) : Material(program, images)
{
    //set specular and shininess parameters
    if(parameters.size()>0)
        m_specularValue = parameters[0];
    if(parameters.size()>1)
        m_shininessValue = parameters[1];

    initUniforms();
}

LitMaterial::~LitMaterial()
{

}

void LitMaterial::initUniforms()
{
    m_uniforms.clear();

    GLuint programId = m_program.lock()->getId();
    m_uniforms.push_back(glGetUniformLocation(programId, "u_ModelMatrix"));
    m_uniforms.push_back( glGetUniformLocation(programId, "u_MVPMatrix"));
    m_uniforms.push_back( glGetUniformLocation(programId, "u_normalMatrix"));

    m_uniforms.push_back( glGetUniformLocation(programId, "u_diffuse"));

    m_uniforms.push_back( glGetUniformLocation(programId, "u_nbPointLight"));
    m_uniforms.push_back( glGetUniformLocation(programId, "u_nbDirectionalLight"));

    for(int i = 0; i < gl::NB_POINT_LIGHT; ++i)
    {
        std::string index = std::to_string(i);
        m_uniforms.push_back( glGetUniformLocation(programId, std::string("u_pointLights["+index+"].position").c_str() ));
        m_uniforms.push_back( glGetUniformLocation(programId, std::string("u_pointLights["+index+"].color").c_str()));
        m_uniforms.push_back( glGetUniformLocation(programId, std::string("u_pointLights["+index+"].radius").c_str()));
        m_uniforms.push_back( glGetUniformLocation(programId, std::string("u_pointLights["+index+"].intensity").c_str()));
    }
    for(int i = 0; i < gl::NB_DIRECTIONAL_LIGHT; ++i)
    {
        std::string index = std::to_string(i);
        m_uniforms.push_back( glGetUniformLocation(programId, std::string("u_directionalLights["+index+"].direction").c_str()));
        m_uniforms.push_back( glGetUniformLocation(programId, std::string("u_directionalLights["+index+"].color").c_str()));
        m_uniforms.push_back( glGetUniformLocation(programId, std::string("u_directionalLights["+index+"].intensity").c_str()));
    }

     m_uniforms.push_back( glGetUniformLocation(programId, "u_viewPosition"));

     m_uniforms.push_back( glGetUniformLocation(programId, "u_specularValue"));
     m_uniforms.push_back( glGetUniformLocation(programId, "u_shininessValue"));

}

void LitMaterial::setUniforms(const glm::mat4& modelMat, const glm::mat4& projectionMat, const glm::mat4& viewMat)
{
    GLuint glId = m_program.lock()->getId();

    //matrix uniforms
    glm::mat4 MV = viewMat * modelMat;
    glm::mat4 MVP = projectionMat * MV; //modelViewProjection
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(modelMat));


    //set program uniforms...
    glUniformMatrix4fv( m_uniforms[0] , 1, false, &modelMat[0][0] );
    glUniformMatrix4fv( m_uniforms[1] , 1, false, &MVP[0][0] );
    glUniformMatrix4fv( m_uniforms[2] , 1, false, &NormalMatrix[0][0] );

    if(m_images.size() > 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_images[0].lock()->getId());
        glUniform1i( m_uniforms[3], 0);
    }

    //no light information given to shaders
    glUniform1ui( m_uniforms[4], 0);
    glUniform1ui( m_uniforms[5], 0);
}

void LitMaterial::setUniforms(const glm::mat4& modelMat, const glm::mat4& projectionMat, const glm::mat4& viewMat, std::shared_ptr<CArray<PointLight>> pointLights, std::shared_ptr<CArray<DirectionalLight>> directionalLights, const glm::vec3& viewPosition)
{
    GLuint glId = m_program.lock()->getId();
    int k = 0;

    //matrix uniforms
    glm::mat4 MV = viewMat * modelMat;
    glm::mat4 MVP = projectionMat * MV; //modelViewProjection
    glm::mat4 NormalMatrix = glm::transpose(glm::inverse(modelMat));


    //set program uniforms...
    glUniformMatrix4fv( m_uniforms[k++] , 1, false, &modelMat[0][0] );
    glUniformMatrix4fv( m_uniforms[k++] , 1, false, &MVP[0][0] );
    glUniformMatrix4fv( m_uniforms[k++] , 1, false, &NormalMatrix[0][0] );

    if(m_images.size() > 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_images[0].lock()->getId());
        glUniform1i( m_uniforms[k], 0);
    }
    k++;

    glUniform1i( m_uniforms[k++], pointLights->size());
    glUniform1i( m_uniforms[k++], directionalLights->size());


    for(int i = 0; i < gl::NB_POINT_LIGHT; ++i)
    {
        if(i < pointLights->size())
        {
            PointLight currentLight = pointLights->parse(i);
            glUniform3fv(m_uniforms[k++], 1, &(currentLight.getPosition())[0]);
            glUniform3fv(m_uniforms[k++], 1, &(currentLight.getColor())[0]);
            glUniform1f(m_uniforms[k++], currentLight.getRadius());
            glUniform1f(m_uniforms[k++], currentLight.getIntensity());
        }
        else
            k+=4;
    }

    for(int i = 0; i < gl::NB_DIRECTIONAL_LIGHT; ++i)
    {
        if(i < directionalLights->size())
        {
            DirectionalLight currentLight = directionalLights->parse(i);
            glUniform3fv(m_uniforms[k++], 1, &(currentLight.getDirection())[0]);
            glUniform3fv(m_uniforms[k++], 1, &(currentLight.getColor())[0]);
            glUniform1f(m_uniforms[k++], currentLight.getIntensity());
        }
        else
            k+=3;
    }

    glUniform3fv(m_uniforms[k++], 1, &viewPosition[0]);

    glUniform1f(m_uniforms[k++], m_specularValue);
    glUniform1f(m_uniforms[k++], m_shininessValue);

}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// UnlitMaterial ////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

SkyboxMaterial::SkyboxMaterial( std::shared_ptr<GLProgram> program ) : Material(program)
{
    initUniforms();
}

SkyboxMaterial::SkyboxMaterial(std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image> > images ) : Material(program)
{
    for(auto image : images)
        m_images.push_back(image);

    initUniforms();
}

SkyboxMaterial::SkyboxMaterial( std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<Image>> images, std::vector<float> parameters) : Material(program)
{
    for(auto image : images)
        m_images.push_back(image);

    initUniforms();
}

SkyboxMaterial::SkyboxMaterial(std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<CubeMap> > cubeMaps ) : Material(program)
{
    for(auto cubeMap : cubeMaps)
        m_cubeMaps.push_back(cubeMap);

    initUniforms();
}

SkyboxMaterial::SkyboxMaterial( std::shared_ptr<GLProgram> program, std::vector<std::shared_ptr<CubeMap>> cubeMaps, std::vector<float> parameters) : Material(program)
{
    for(auto cubeMap : cubeMaps)
        m_cubeMaps.push_back(cubeMap);

    initUniforms();
}

SkyboxMaterial::~SkyboxMaterial()
{

}

void SkyboxMaterial::pushToGPU()
{
    Material::pushToGPU(); //push images if any

    for(auto cubeMap : m_cubeMaps)
    {
        ResourceManager::getInstance().pushCubeMapToGPU(cubeMap.lock()->getName());
    }

}

void SkyboxMaterial::popFromGPU()
{
    Material::popFromGPU(); //popImages if any

    for(auto cubeMap : m_cubeMaps)
    {
        ResourceManager::getInstance().popCubeMapFromGPU(cubeMap.lock()->getName());
    }
}

void SkyboxMaterial::initUniforms()
{
    GLuint programId = m_program.lock()->getId();
    m_uniforms.push_back(glGetUniformLocation(programId, "u_viewMat"));
    m_uniforms.push_back(glGetUniformLocation(programId, "u_projectionMat"));
    m_uniforms.push_back(glGetUniformLocation(programId, "u_diffuse"));
}

void SkyboxMaterial::setUniforms(const glm::mat4& modelMat, const glm::mat4& projectionMat, const glm::mat4& viewMat)
{
    GLuint glId = m_program.lock()->getId();

    //matrix uniforms
    glm::mat4 view =  glm::mat4( glm::mat3(viewMat) ); //projection * view

    //set program uniforms...
    glUniformMatrix4fv( m_uniforms[0] , 1, false, &view[0][0] );
    glUniformMatrix4fv( m_uniforms[1] , 1, false, &projectionMat[0][0] );


    if(m_cubeMaps.size() > 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMaps[0].lock()->getId());
        glUniform1i( m_uniforms[2], 0);
    }

}

void SkyboxMaterial::setUniforms(const glm::mat4& modelMat, const glm::mat4& projectionMat, const glm::mat4& viewMat, std::shared_ptr<CArray<PointLight>> pointLights, std::shared_ptr<CArray<DirectionalLight>> directionalLights, const glm::vec3& viewPosition)
{
    GLuint glId = m_program.lock()->getId();

    //matrix uniforms
    glm::mat4 view =  glm::mat4( glm::mat3(viewMat) ); //projection * view


    //set program uniforms...
    glUniformMatrix4fv( m_uniforms[0] , 1, false, &view[0][0] );
    glUniformMatrix4fv( m_uniforms[1] , 1, false, &projectionMat[0][0] );


    if(m_cubeMaps.size() > 0)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMaps[0].lock()->getId());
        glUniform1i( m_uniforms[2], 0);
    }
}


}
