
#include "thotEngine/Renderer.hpp"

namespace te{

void Renderer::render(glm::mat4 const& worldMat, glm::mat4 const& viewMat, std::shared_ptr<CArray<MeshRenderer>> meshes)
{
    GLuint matID = -1;


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

}
