#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <stdlib.h>
#include <stdio.h>
#include <memory>

#include <SDL\SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "thotEngine/CArray.hpp"
#include "thotEngine/MeshRenderer.hpp"
#include "thotEngine/Camera.hpp"

#include "thotEngine/GLProgram.hpp"
#include "thotEngine/MaterialManager.hpp"

namespace te{

class Renderer
{

private:
    //deferred rendering parameters :
    GLuint m_gBuffer;
    GLuint m_gTexPosition;
    GLuint m_gTexNormal;
    GLuint m_gTexAlbedo;
    GLuint m_gTexSpecShininess;

    std::vector<GLuint> m_uniforms;

    GLProgram m_gProgram;
    GLProgram m_lProgram;

    Mesh m_postProdQuad;

    std::weak_ptr<LightPassMaterial> m_lightPassMaterial;

public :
    Renderer();
    ~Renderer();

    void render(glm::mat4 const& worldMat, glm::mat4 const& viewMat, std::shared_ptr<CArray<MeshRenderer>> meshes);
    void render(Camera& camera, std::shared_ptr<CArray<MeshRenderer>> meshes, std::shared_ptr<CArray<PointLight> > pointLights, std::shared_ptr<CArray<DirectionalLight> > directionalLights);

    //deferred rendering :
    void deferred_render(Camera& camera, std::shared_ptr<CArray<MeshRenderer>> meshes, std::shared_ptr<CArray<PointLight>> pointLights, std::shared_ptr<CArray<DirectionalLight>> directionalLights);
    void initDeferredUniforms();
    void setDeferredUniforms(glm::vec3 viewPosition, std::shared_ptr<CArray<PointLight>> pointLights, std::shared_ptr<CArray<DirectionalLight>> directionalLights);

    /**
     * @brief lightPass
     * @param meshes
     * @param pointLights
     * @param directionalLights
     *
     * update shadowMap of each lights on scene
     */
    void lightPass(std::shared_ptr<CArray<MeshRenderer>> meshes, std::shared_ptr<CArray<PointLight>> pointLights, std::shared_ptr<CArray<DirectionalLight>> directionalLights); //update shadowMaps for each light
};

}

#endif // RENDERER_HPP
