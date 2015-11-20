#include "thotEngine/Transform.hpp"
#include "thotEngine/WindowManager.hpp"
#include "thotEngine/GLConfig.hpp"

#include "thotEngine/Renderer.hpp"

namespace te{

Renderer::Renderer()
{
    //frameBuffer :
    glGenFramebuffers(1, &m_gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

    //position
    glGenTextures(1, &m_gTexPosition);
    glBindTexture(GL_TEXTURE_2D, m_gTexPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WindowManager::getWindowDim().x, WindowManager::getWindowDim().y, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gTexPosition, 0);
    //glBindTexture(GL_TEXTURE_2D, 0);

    //normals
    glGenTextures(1, &m_gTexNormal);
    glBindTexture(GL_TEXTURE_2D, m_gTexNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WindowManager::getWindowDim().x, WindowManager::getWindowDim().y, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gTexNormal, 0);
    //glBindTexture(GL_TEXTURE_2D, 0);

    //diffuse
    glGenTextures(1, &m_gTexAlbedo);
    glBindTexture(GL_TEXTURE_2D, m_gTexAlbedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WindowManager::getWindowDim().x, WindowManager::getWindowDim().y, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gTexAlbedo, 0);
    //glBindTexture(GL_TEXTURE_2D, 0);

    //diffuse and specular
    glGenTextures(1, &m_gTexSpecShininess);
    glBindTexture(GL_TEXTURE_2D, m_gTexSpecShininess);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WindowManager::getWindowDim().x, WindowManager::getWindowDim().y, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_gTexSpecShininess, 0);
    //glBindTexture(GL_TEXTURE_2D, 0);

    GLuint attachments[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(4, attachments);

    GLuint rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowManager::getWindowDim().x, WindowManager::getWindowDim().y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    //Check frameBuffer validity
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //programs :
    m_gProgram.init("gProgram", "shaders/def_gPass.vs.glsl", "shaders/def_gPass.fs.glsl");
    m_lProgram.init("lProgram", "shaders/def_lPass.vs.glsl", "shaders/def_lPass.fs.glsl");

    //mesh :
    m_postProdQuad.init({
                            Vertex( glm::vec3(-1, 1, 0), glm::vec3(0,1,0), glm::vec2(0,1)),
                            Vertex( glm::vec3(-1, -1, 0), glm::vec3(0,1,0), glm::vec2(0,0)),
                            Vertex( glm::vec3(1, 1, 0), glm::vec3(0,1,0), glm::vec2(1,1)),
                            Vertex( glm::vec3(1, -1, 0), glm::vec3(0,1,0), glm::vec2(1,0))
                        },
                        {
                            0,1,2,2,1,3
                        });
    m_postProdQuad.pushToGPU();

    //init uniforms :
    initDeferredUniforms();

}

Renderer::~Renderer()
{

}

void Renderer::render(glm::mat4 const& worldMat, glm::mat4 const& viewMat, std::shared_ptr<CArray<MeshRenderer>> meshes)
{
    GLuint matID = -1;

    //clear
    glClear(GL_DEPTH_BUFFER_BIT || GL_COLOR_BUFFER_BIT);

    for(int i = 0; i < meshes->size(); ++i)
    {
        //change current shader program if needed
        if(meshes->parse(i).getMaterialID() != matID)
        {
            matID = meshes->parse(i).getMaterialID();
            glUseProgram(matID);
        }

        meshes->parse(i).render(worldMat, viewMat);
    }
}

void Renderer::render(Camera& camera, std::shared_ptr<CArray<MeshRenderer>> meshes, std::shared_ptr<CArray<PointLight>> pointLights, std::shared_ptr<CArray<DirectionalLight>> directionalLights)
{
    GLuint matID = -1;

    camera.updateViewMatrix();
    glm::mat4 worldMat = camera.getWorldMatrix();
    glm::mat4 viewMat = camera.getViewMatrix();
    glm::vec3 viewPosition = camera.transform()->getTranslation();

    //clear
    glEnable(GL_DEPTH_TEST);
    //glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    camera.clear();


    for(int i = 0; i < meshes->size(); ++i)
    {
        //change current shader program if needed
        if(meshes->parse(i).getMaterialID() != matID)
        {
            matID = meshes->parse(i).getMaterialID();
            glUseProgram(matID);
        }

        meshes->parse(i).render(worldMat, viewMat, pointLights, directionalLights, viewPosition);
    }
}

void Renderer::deferred_render(Camera& camera, std::shared_ptr<CArray<MeshRenderer>> meshes, std::shared_ptr<CArray<PointLight>> pointLights, std::shared_ptr<CArray<DirectionalLight>> directionalLights)
{
    GLuint matID = -1;

    camera.updateViewMatrix();
    glm::mat4 worldMat = camera.getWorldMatrix();
    glm::mat4 viewMat = camera.getViewMatrix();
    glm::vec3 viewPosition = camera.transform()->getTranslation();

    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_gProgram.getId());
    for(int i = 0; i < meshes->size(); ++i)
    {
         meshes->parse(i).render(worldMat, viewMat, pointLights, directionalLights, viewPosition);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //glDisable(GL_DEPTH_TEST);

    //clear
    camera.clear();
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_lProgram.getId());
    setDeferredUniforms(viewPosition, pointLights, directionalLights);
    m_postProdQuad.draw();

//    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer);
//    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//    glBlitFramebuffer( 0, 0, WindowManager::getWindowDim().x, WindowManager::getWindowDim().y, 0, 0, WindowManager::getWindowDim().x, WindowManager::getWindowDim().y, GL_DEPTH_BUFFER_BIT, GL_NEAREST );
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);



}

void Renderer::initDeferredUniforms()
{
    GLuint programId = m_lProgram.getId();
    m_uniforms.push_back( glGetUniformLocation(programId, "u_nbPointLight"));
    m_uniforms.push_back( glGetUniformLocation(programId, "u_nbDirectionalLight"));

    for(int i = 0; i <gl::NB_POINT_LIGHT; ++i)
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

     m_uniforms.push_back(glGetUniformLocation(programId, "u_fragPos"));
     m_uniforms.push_back(glGetUniformLocation(programId, "u_normals"));
     m_uniforms.push_back(glGetUniformLocation(programId, "u_albedo"));
     m_uniforms.push_back(glGetUniformLocation(programId, "u_specShininess"));

}

void Renderer::setDeferredUniforms(glm::vec3 viewPosition, std::shared_ptr<CArray<PointLight>> pointLights, std::shared_ptr<CArray<DirectionalLight>> directionalLights)
{
    int k = 0;
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

    //active and send texture units :
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_gTexPosition);
    glUniform1i(m_uniforms[k++], 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_gTexNormal);
    glUniform1i(m_uniforms[k++], 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_gTexAlbedo);
    glUniform1i(m_uniforms[k++], 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_gTexSpecShininess);
    glUniform1i(m_uniforms[k++], 3);

}

}
