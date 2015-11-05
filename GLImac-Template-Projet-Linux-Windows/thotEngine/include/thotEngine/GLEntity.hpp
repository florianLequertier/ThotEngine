#ifndef GLENTITY_HPP
#define GLENTITY_HPP

#include <iostream>
#include <cstdlib>
#include <vector>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace te{

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 color;

    Vertex() : position(0, 0, 0), normal(0,0,0), texCoords(0,0), color(1, 1, 1)
    {

    }

    Vertex(glm::vec3 pos, glm::vec3 norm = glm::vec3(0,1,0), glm::vec2 tex = glm::vec2(0,0) ,glm::vec3 col = glm::vec3(1,1,1)) : position(pos), normal(norm), texCoords(tex), color(col)
    {

    }

};

struct UncoloredVertex
{
    glm::vec3 position;

    UncoloredVertex() : position(0, 0, 0)
    {

    }

    UncoloredVertex(glm::vec3 pos) : position(pos)
    {

    }

};

class GLEntity
{
private :
    GLuint m_vbo;
    GLuint m_ibo;
    GLuint m_vao;

    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;

    unsigned int m_indexNumber;
    unsigned int m_materialIndex;
public:
    GLEntity();
    ~GLEntity();
    GLEntity(const GLEntity& other);
    GLEntity& operator=(const GLEntity& other);

    void init(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    void clear();

    void pushToGPU();
    void popFromGPU();

    void draw();
    unsigned int getMaterialIndex() const;
    void setMaterialIndex(unsigned int index);
};

}

#endif // GLENTITY_HPP
