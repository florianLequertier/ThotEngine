#include "thotEngine/Transform.hpp"

#include "thotEngine/Renderer.hpp"

namespace te{

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

}
