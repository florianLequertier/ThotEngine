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

namespace te{

class Renderer
{

public :
    void render(glm::mat4 const& worldMat, glm::mat4 const& viewMat, std::shared_ptr<CArray<MeshRenderer>> meshes);

};

}

#endif // RENDERER_HPP
